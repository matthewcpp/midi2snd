#define TSF_IMPLEMENTATION
#include "tsf.h"

#define TML_IMPLEMENTATION
#include "tml.h"

#include <sndfile.h>

#include <vector>
#include <iostream>
#include <string>

int convert_midi_file(tml_message* midi_file, tsf* soundfont, std::string const & dest_file_path);

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Example usage:" << std::endl;
        std::cout << "./midi2snd midi_file sound_font ogg_file" << std::endl;
        return 1;
    }

    std::string midi_file_path = argv[1];
    std::string soundfont_file_path = argv[2];
    std::string dest_file_path = argv[3];

    tml_message* midi_file = tml_load_filename(midi_file_path.c_str());

    if (!midi_file) {
        std::cout << "Unable to load file: " << midi_file_path << std::endl;
        return 1;
    }

    tsf* soundfont = tsf_load_filename(soundfont_file_path.c_str());

    if (!soundfont) {
        tsf_close(soundfont);
        std::cout << "Unable to load soundfont: " << soundfont_file_path;
        return 1;
    }

    int result = convert_midi_file(midi_file, soundfont, dest_file_path);

    tsf_close(soundfont);
    tml_free(midi_file);

    return result;
}

int convert_midi_file(tml_message* midi_file, tsf* soundfont, std::string const & dest_file_path) {
    const int sample_rate = 44100;
    const int channel_count = 2;
    const int sample_count = 4096;
    std::vector<float> sound_data(channel_count * sample_count);

    SF_INFO dest_info;
    dest_info.samplerate = sample_rate;
    dest_info.channels = channel_count;
    dest_info.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
    SNDFILE* dest_file = sf_open(dest_file_path.c_str(), SFM_WRITE, &dest_info);

    if (!dest_file) {
        std::cout << "Unable to open output: " << dest_file_path << std::endl;
        std::cout << sf_strerror(NULL);
        return 1;
    }

    tml_message* current_message = midi_file;
    double current_playback_time = 0.0f;

    while (current_message) {
        int samples_remaining = sample_count;
        float* data_ptr = sound_data.data();

        for (int sample_block = TSF_RENDER_EFFECTSAMPLEBLOCK; samples_remaining > 0; samples_remaining -= sample_block) {

            //We progress the MIDI playback and then process TSF_RENDER_EFFECTSAMPLEBLOCK samples at once
            if (sample_block > samples_remaining)
                sample_block = samples_remaining;

            //Loop through all MIDI messages which need to be played up until the current playback time
            for (current_playback_time += sample_block * (1000.0 / static_cast<double>(sample_rate)); current_message && current_playback_time >= current_message->time; current_message = current_message->next)
            {
                switch (current_message->type)
                {
                    case TML_PROGRAM_CHANGE: //channel program (preset) change (special handling for 10th MIDI channel with drums)
                        tsf_channel_set_presetnumber(soundfont, current_message->channel, current_message->program, (current_message->channel == 9));
                        break;
                    case TML_NOTE_ON: //play a note
                        tsf_channel_note_on(soundfont, current_message->channel, current_message->key, current_message->velocity / 127.0f);
                        break;
                    case TML_NOTE_OFF: //stop a note
                        tsf_channel_note_off(soundfont, current_message->channel, current_message->key);
                        break;
                    case TML_PITCH_BEND: //pitch wheel modification
                        tsf_channel_set_pitchwheel(soundfont, current_message->channel, current_message->pitch_bend);
                        break;
                    case TML_CONTROL_CHANGE: //MIDI controller messages
                        tsf_channel_midi_control(soundfont, current_message->channel, current_message->control, current_message->control_value);
                        break;
                }
            }

            // Render the block of audio samples in float format
            tsf_render_float(soundfont, data_ptr, sample_block, 0);
            data_ptr += sample_block * 2;
        }

        sf_write_float(dest_file, sound_data.data(), sound_data.size());
    }

    sf_close(dest_file);

    return 0;
}