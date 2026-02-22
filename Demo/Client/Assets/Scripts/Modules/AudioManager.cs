using System;
using System.Collections.Generic;
using AutoConfig;
using Unity.Properties;
using UnityEngine;
using XEngine;
using XGame;
using XUtils;

public interface IAudioManager : IModule
{
    public void PlaySound(string soundPath);
    public void PlayMusic(string musicPath, bool loop = true);
    public void StopMusic();
    public void StopAllSound();
    public void PauseMusic();
}

public class AudioManager : IAudioManager
{
    private GameObject _AudioRoot;
    private AudioSource _MusicSource;
    private List<AudioSource> _SoundSources = new List<AudioSource>();

    private const int MAX_SOUND_SOURCES = 10;

    IGameSceneManager _GameSceneManager;

    [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.SubsystemRegistration)]
    static void Regist()
    {
        Launcher.RegistModule<IAudioManager, AudioManager>();
    }
    
    public bool Initialize()
    {
        _AudioRoot = new GameObject("AudioManager");
        UnityEngine.Object.DontDestroyOnLoad(_AudioRoot);

        //创建音乐播放器组件
        _MusicSource = _AudioRoot.AddComponent<AudioSource>();
        _MusicSource.playOnAwake = false;
        _MusicSource.loop = true;

        //创建音效播放器组件
        for (int i = 0; i < MAX_SOUND_SOURCES; i++)
        { 
            AudioSource source = _AudioRoot.AddComponent<AudioSource>();
            source.playOnAwake = false;
            _SoundSources.Add(source);
        }
        return true;
    }

    public bool Launch()
    {
        _GameSceneManager = Launcher.GetModule<IGameSceneManager>();
        _GameSceneManager.RegistEvent(IGameSceneManager.eEvent.GameMapLoaded, OnSceneLoaded);
        return true;
    }

    private void OnSceneLoaded(UnityEngine.SceneManagement.Scene scene)
    {
        AutoConfig.Scene currentSceneConfig = _GameSceneManager.GetCurrentSceneConfig();
        if (currentSceneConfig == null)
        { 
            return;
        }

        PlayMusic(AutoConfig.Sound.Get(currentSceneConfig._Sound)._AudioMaterial, AutoConfig.Sound.Get(currentSceneConfig._Sound)._Loop);
    }

    public void Release()
    {

    }

    private AudioSource GetAvailableSoundSource()
    {
        foreach (var source in _SoundSources)
        {
            if (!source.isPlaying)
            {
                return source;
            }
        }

        return _SoundSources.Count > 0 ? _SoundSources[0] : null;
    }

    public void PlaySound(string soundPath)
    {
        if (string.IsNullOrEmpty(soundPath))
        {
            return;
        }
        
        var source = GetAvailableSoundSource();
        if (source == null) return;

        var clip = LoadAudioClip(soundPath);
        if (clip == null)
        {
            return ;
        }

        //播放音效
        source.clip = clip;
        source.Play();
    }

    public void PlayMusic(string musicPath, bool loop = true)
    { 
        if(string.IsNullOrEmpty(musicPath)) return;

        var clip = LoadAudioClip(musicPath);
        if (clip == null) return;

        _MusicSource.clip = clip;
        _MusicSource.loop = loop;

        _MusicSource.Play();
    }

    public void StopMusic()
    { 
        if(!_MusicSource.isPlaying) return;

        _MusicSource.Stop();
    }

    public void PauseMusic()
    { 
        if(!_MusicSource.isPlaying) return;

        _MusicSource.Pause();
    }

    public void StopAllSound()
    {
        foreach (var source in _SoundSources)
        {
            if (source.isPlaying)
            {
                source.Stop();
            }
        }

        if (_MusicSource.isPlaying)
        {
            _MusicSource.Stop();
        }
    }

    private AudioClip LoadAudioClip(string clipPath)
    {
        var clip = Resources.Load<AudioClip>(clipPath);
        if (clip == null)
        {
            XApi.Error("音频资源未找到{clipPath}", clipPath);
            return null;
        }

        return clip;
    }
}
