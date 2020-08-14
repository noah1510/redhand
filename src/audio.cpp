#include "redhand/audio.hpp"

using namespace redhand;

audio::audio()
{
    isInitialized = false;
    int initialized = audio::InitAL();
    if(initialized == 0)
        isInitialized = true;
    else
    {
        std::cerr << "Could not initialize the audio system!\n" << std::endl;
    }
    
}

int audio::InitAL()
{

    const ALCchar *name;
    ALCdevice *device;
    ALCcontext *ctx;

    /* Open and initialize a device */
    device = alcOpenDevice(NULL);
    if(!device)
    {
        std::cerr << "Could not open a device!\n" << std::endl;

        return 1;
    }

    ctx = alcCreateContext(device, NULL);
    if(ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if(ctx != NULL)
            alcDestroyContext(ctx);
        alcCloseDevice(device);
        std::cerr << "Could not set a context!\n" << std::endl;
        return 1;
    }

    name = NULL;
    if(alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if(!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    std::cout << "Opened " << "\"" << name <<  "\"" << std::endl;

    return 0;
}

void audio::CloseAL(void)
{
    ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if(ctx == NULL)
        return;

    device = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(device);
}

/* LoadBuffer loads the named audio file into an OpenAL buffer object, and
 * returns the new buffer ID.
 */
ALuint audio::LoadSound(const char *filename)
{
    ALenum err, format;
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    short *membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;
    
    /* Open the audio file and check that it's usable. */
    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if(!sndfile)
    {
        std::cerr << "Could not open audio in " << filename << ": "<< sf_strerror(sndfile) << std::endl;
        return 0;
    }
    if(sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX/sizeof(short))/sfinfo.channels)
    {
        std::cerr << "Bad sample count in %s (%" PRId64 ")" << filename << sfinfo.frames << std::endl;
        sf_close(sndfile);
        return 0;
    }
    /* Get the sound format, and figure out the OpenAL format */
    if(sfinfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if(sfinfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr << "Unsupported channel count: %d\n" << sfinfo.channels << std::endl;
        sf_close(sndfile);
        return 0;
    }

    /* Decode the whole audio file to a buffer. */
    membuf = (short*) malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short));

    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if(num_frames < 1)
    {
        free(membuf);
        sf_close(sndfile);
        std::cerr << "Failed to read samples in %s (%" PRId64 ")\n" << filename << num_frames << std::endl;
        return 0;
    }
    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

    /* Buffer the audio data into a new buffer object, then free the data and
     * close the file.
     */
    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if(err != AL_NO_ERROR)
    {
        std::cerr << "OpenAL Error: %s\n" << alGetString(err) << std::endl;
        if(buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return 0;
    }

    std::cout << "The audio file was loaded successfully!" << std::endl;
    return 0;
}
void audio::al_nssleep(unsigned long nsec)
{
    struct timespec ts, rem;
    ts.tv_sec = (time_t)(nsec / 1000000000ul);
    ts.tv_nsec = (long)(nsec % 1000000000ul);
    while(nanosleep(&ts, &rem) == -1 && errno == EINTR)
        ts = rem;
}

void audio::play()
{
    ALuint source;
    ALfloat offset;
    ALenum state;

    /* Load the sound into a buffer. */
    if(!buffer)
    {
        CloseAL();
    }

    /* Create the source to play the sound with. */
    source = 0;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, (ALint)buffer);
    assert(alGetError()==AL_NO_ERROR && "Failed to setup sound source");

    /* Play the sound until it finishes. */
    alSourcePlay(source);
    do {
        al_nssleep(10000000);
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        /* Get the source offset. */
        alGetSourcef(source, AL_SEC_OFFSET, &offset);
        printf("\rOffset: %f  ", offset);
        fflush(stdout);
    } while(alGetError() == AL_NO_ERROR && state == AL_PLAYING);
    printf("\n");

    /* All done. Delete resources, and close down OpenAL. */
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

    audio::CloseAL();
}