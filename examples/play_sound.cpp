//
// Created by William.Hua on 2021/2/27.
//

#include "libaa/fileio/aa_mp3_audio_format_reader.h"
#include "libaa/fileio/aa_file_stream.h"
#include "portaudio.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
using namespace std;
using namespace libaa;

class DecoderData
{
public:
    DecoderData(Mp3AudioFormatReader& r, int frames_per_buffer):
        reader(r),
        left_buffer(frames_per_buffer, 0.0f),
        right_buffer(frames_per_buffer, 0.0f)
    {
        data_refer_to[0] = left_buffer.data();
        data_refer_to[1] = right_buffer.data();
    }

    Mp3AudioFormatReader& reader;
    vector<float> left_buffer;
    vector<float> right_buffer;
    float* data_refer_to[2]{};
};


static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    auto *data = (DecoderData*)userData;
    float *out = (float*)outputBuffer;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    auto read_ok = data->reader.readSamples(data->data_refer_to, 2, 0, 0, framesPerBuffer);


    for(auto i=0; i < framesPerBuffer; i++ )
    {
        *out++ = data->data_refer_to[0][i];   /* left */
        *out++ = data->data_refer_to[1][i];  /* right */
    }

    if(!read_ok){
        return paComplete;
    }

    return paContinue;
}


/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
    printf( "Stream Completed: %s\n", "data->message" );
}

int main(int argc, char* argv[])
{
    if(argc < 2){
        cerr << "Usage: " << argv[0] << "/path/to/audio/file" << endl;
        return -1;
    }

    const auto input_file_path = string(argv[1]);

    auto in_stream = std::unique_ptr<InputStream>(new FileStream(input_file_path));

    Mp3AudioFormatReader reader(std::move(in_stream));
    if(!reader.isOpenOk()){
        cerr << "open file failed\n";
        return -1;
    }

    const auto sample_rate = reader.sample_rate;
    const auto num_channels = reader.num_channels;
    const auto num_frames = reader.length_in_samples;
    const auto duration = double(num_frames) / sample_rate;
    const int frames_per_buffer = 64;
    assert(num_channels == 2);

    DecoderData data(reader, frames_per_buffer);


    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        sample_rate,
        frames_per_buffer,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        patestCallback,
        &data );
    if( err != paNoError ) goto error;

    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %lf seconds.\n", duration );
    Pa_Sleep( duration * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");

    return err;
    error:
    Pa_Terminate();
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
