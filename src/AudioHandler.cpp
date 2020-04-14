#include "redhand/AudioHandler.hpp"

using namespace redhand;

AudioHandler::AudioHandler() : device{NULL}, error{NULL}{}

/**
 * Gives you a list of available audio devices.
 * 
 * @param devices Pass your variable which is going to hold the audio devices.
 */
void AudioHandler::listAudioDevices(const ALCchar *devices)
{
    const ALCchar *device = devices, *next = devices + 1;
    size_t len = 0;

    std::cout << "Devices List: " << std::endl;
    std::cout << "----------"     << std::endl;
    while (device && *device != '\0' && next && *next != '\0') {
        std::cout << device << std::endl;
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
    
    std::cout << "----------" << std::endl;
}

inline ALenum AudioHandler::to_al_format(short channels, short samples)
{
    bool stereo = (channels > 1);

    switch (samples) {
    case 16:
            if (stereo)
                    return AL_FORMAT_STEREO16;
            else
                    return AL_FORMAT_MONO16;
    case 8:
            if (stereo)
                    return AL_FORMAT_STEREO8;
            else
                    return AL_FORMAT_MONO8;
    default:
            return -1;
    }
}

#define TEST_ERROR(_msg)		\
    error = alGetError();		\
    if (error != AL_NO_ERROR) {	\
        std::cout << _msg << std::endl; \
    }

void AudioHandler::checkErrors()
{
    error = alGetError();
    if (error != AL_NO_ERROR)
        std::cout << "There was an error" << std::endl;
}

void AudioHandler::createDefaultDevice(ALCdevice *defaultDevice)
{
    ALboolean enumeration;
    const ALCchar *defaultDeviceName;

    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
        std::cout << "enumeration extension not available" << std::endl;

    listAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
    
    defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    defaultDevice = alcOpenDevice(defaultDeviceName);
    if (!defaultDevice) 
    {
        std::cout << "unable to open default device" << std::endl;
    }

    std::cout << "Device: " + (std::string) alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
}

void AudioHandler::playSound(const char* sound_dir)
{   
    ALint source_state;
    ALCcontext *context;
    ALuint source;
    ALuint buffer;


    if (!device)
        createDefaultDevice(device);

    // Test before any openal functions should be called 
    alGetError();


    // Create default context
    context = alcCreateContext(device, NULL);

    if (!alcMakeContextCurrent(context))
        std::cout << "Failed to make default context" << std::endl;

    TEST_ERROR("Context creation");


    alGenSources((ALuint)1, &source);
    TEST_ERROR("Source generation");

    alSourcef(source, AL_PITCH, 1);
    TEST_ERROR("Source pitch");
    alSourcef(source, AL_GAIN, 1);
    TEST_ERROR("Source gain");
    alSource3f(source, AL_POSITION, 0, 0, 0);
    TEST_ERROR("Source position");
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    TEST_ERROR("Source velocity");
    alSourcei(source, AL_LOOPING, AL_FALSE);
    TEST_ERROR("Source looping");


    alGenBuffers((ALuint)1, &buffer);
    TEST_ERROR("buffer generation");


    WaveInfo *wave;
    char *bufferData;
    int ret;

    wave = WaveOpenFileForReading(sound_dir);
    if (!wave) {
        std::cerr << "failed to read wave file" << std::endl;
    }

    ret = WaveSeekFile(0, wave);
    if (ret) {
        std::cerr << "failed to seek wave file" << std::endl;
    }

    bufferData = (char *) malloc(wave->dataSize);
    if (!bufferData) {
        perror("malloc");
    }

    ret = WaveReadFile(bufferData, wave->dataSize, wave);
    if (ret != (int) wave->dataSize) {
        std::cerr <<  "short read: " << (double) ret << ", want: " << (double) wave->dataSize << std::endl;
    }

    alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
                bufferData, wave->dataSize, wave->sampleRate);
    TEST_ERROR("Failed to load buffer data");



    alSourcei(source, AL_BUFFER, buffer);
    TEST_ERROR("Buffer binding");

    alSourcePlay(source);
    TEST_ERROR("Source playing");

    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	TEST_ERROR("Get source state");

	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("Get source state");
	}

    alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}



