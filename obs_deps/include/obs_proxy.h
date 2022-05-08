//
// Created by 李文禹 on 2022/1/24.
//

#ifndef OBS_STUDIO_OBS_PROXY_H
#define OBS_STUDIO_OBS_PROXY_H

#include <obs.hpp>
#include <util/lexer.h>
#include <util/profiler.h>
#include <util/util.hpp>
#include <util/platform.h>
#include <graphics/graphics.h>
#include <graphics/vec4.h>
#include <graphics/matrix4.h>
#include <functional>
#include <string>
#include <memory>
#include <vector>
#include <deque>


#ifdef _MSC_VER
#define OBS_PROXY_EXPORT  __declspec(dllexport)
#else
#define OBS_PROXY_EXPORT
#endif

struct BasicOutputHandler;
class OBSSceneManager;

#define Live_Title	"live.title"
#define Live_Speaker_Name	"live.speaker.name"
#define Live_Speaker_Intro	"live.speaker.intro"

//obs插件附加搜索目录
struct ExtraModulePath {
	char* pluginPath = nullptr;
	char* pluginDataPath = nullptr;
};


class IOBSProxyEventHandler
{
public:
	virtual ~IOBSProxyEventHandler() = default;
	virtual void onEvent(const char* event, const char* data) = 0;
};


class OBS_PROXY_EXPORT OBSProxyAPICall {

};


class OBS_PROXY_EXPORT OBSProxy {
public:
	~OBSProxy();

	static OBSProxy& GetInstance();

	static void ReleaseInstance();

	static OBSProxy* instance;

	//初始化libobs
	bool StartOBS();

	//加载obs模块,可以设置ExtraModulePath添加插件模块搜索路径
	void InitOBS(ExtraModulePath* extraModulePath[]);

	bool AddSource(const char* json);

	//添加资源，data包含资源的相关信息
	bool AddSource(obs_data_t* data);

	//获取推流服务
	obs_service_t* GetService();

	/* 设置rtmp service参数
	 * server rtmp服务地址
	 * key 流名称
	 * 推srs情况下， server: rtmp://192.168.3.72:1935/live   key: test
	 * 推七牛的情况下， 直接把七牛的地址赋值给server,key为空 */
	void SetServiceSettings(obs_data_t *setting);

	obs_scene_t* GetCurrentScene();

	obs_source_t* GetSourceByName(const char* name);

	//切换到指定场景
	bool TransitionToScene(const char* name);

	//libobs core 是否已经启动
	bool IsOBSStarted() const {
		return isOBSStarted;
	}

	//获取语言包中的键值所对应的内容
	const char* GetLanguageString(const char *key) const;


	//开启本地录制
	bool StartRecording();

	//停止本地录制
	void StopRecording();

	// 开启推流
	bool StartStreaming();

public:
	//obs信号处理
	static void SourceCreated(void *data,calldata_t *paramaters);
	static void SourceRemove(void *data,calldata_t *paramaters);
	static void SourceRename(void *data,calldata_t *paramaters);
	static void SourceActivate(void *data,calldata_t *paramaters);
	static void SourceDeActivate(void *data,calldata_t *paramaters);

	//麦克风音量回调
	static void OBSMicVolumeLevel(void *data,
				   const float magnitude[MAX_AUDIO_CHANNELS],
				   const float peak[MAX_AUDIO_CHANNELS],
				   const float inputPeak[MAX_AUDIO_CHANNELS]);

	static void OBSAudioDataCapture(void *param, obs_source_t *source,
					const struct audio_data *audio_data,
					bool muted);

protected:
	//重置输出流
	bool ResetOutputs();

	//重置音频
	bool ResetAudio();

	//重置视频
	int ResetVideo();

	/* 初始化rtmp推流服务 */
	bool InitService();

	//初始化图形单元
	void InitGraphicsPrimitives();

	//初始化obs信号处理（资源添加、删除、重命名、激活、取消激活）
	void InitOBSCallBacks();

	//创建默认的场景资源
	void CreateDefaultSceneSource();

	//遍历创建当前支持的场景转换资源
	void InitDefaultTransitions();

	//遍历创建所有的filter滤镜资源
	void InitDefaultFilters();

	//初始化语言包
	bool InitLocale();

	//添加示例数据，只是用来测试
	void addExampleSource();

	//测试验证channel
	void addTestChannel();

	//测试验证音频
	void addTestAudioChannel();

private:
	OBSProxy();

private:
	//推流服务
	OBSService service;

	//输出流
	std::unique_ptr<BasicOutputHandler> outputHandler;

	//语言包
	TextLookup textLookup;

	//libobs是否已经初始化
	bool isOBSStarted = false;

	//obs信号
	std::vector<OBSSignal> signalHandlers;

	//当前场景
	OBSScene currentScene = nullptr;

	//场景切换使用的过度动画,切换场景只需要修改过度动画的transitionsource就可以了，
	//调用obs_transition_set
	OBSSource currentTransition = nullptr;

	//所有可用的transition资源
	std::vector<OBSSource> allTransitions;

	//所有可用的filter资源
	std::vector<OBSSource> allFilters;

	//
	std::string waitTransitionScene = "";

	obs_fader_t *audioFader = nullptr;

	obs_volmeter_t *audioVolmeter = nullptr;

	gs_vertbuffer_t *box = nullptr;
	gs_vertbuffer_t *boxLeft = nullptr;
	gs_vertbuffer_t *boxTop = nullptr;
	gs_vertbuffer_t *boxRight = nullptr;
	gs_vertbuffer_t *boxBottom = nullptr;
	gs_vertbuffer_t *circle = nullptr;

	gs_vertbuffer_t *actionSafeMargin = nullptr;
	gs_vertbuffer_t *graphicsSafeMargin = nullptr;
	gs_vertbuffer_t *fourByThreeSafeMargin = nullptr;
	gs_vertbuffer_t *leftLine = nullptr;
	gs_vertbuffer_t *topLine = nullptr;
	gs_vertbuffer_t *rightLine = nullptr;


};

#endif //OBS_STUDIO_OBS_PROXY_H
