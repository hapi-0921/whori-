class GameManager
{
public:
    static GameManager& Instance()
    {
        static GameManager instance;
        return instance;
    }

    void SetPlaying(bool playing)
    {
        isPlaying = playing;
    }

    bool IsPlaying() const
    {
        return isPlaying;
    }

private:
    bool isPlaying = false;

public:
    bool needCameraReset = false;

};