//
// Created by 李文禹 on 2022/1/25.
//

#ifndef OBS_STUDIO_GENERAL_CONFIG_H
#define OBS_STUDIO_GENERAL_CONFIG_H

#include <obs.hpp>
#include <util/threading.h>
#include <util/platform.h>
#include <util/util.hpp>
#include <util/dstr.hpp>
#include <mutex>

#define App_Language_Key	"Language"

#define Basic_Max_Logs	"MaxLogs"

#define Audio_Disable_Ducking	"DisableDucking"
#define Audio_Monitoring_DeviceName	"MonitoringDeviceName"
#define Audio_Monitoring_DeviceID	"MonitoringDeviceId"
#define Audio_SampleRate	"SampleRate"
#define Audio_ChannelSetup	"ChannelSetup"

#define Video_Render_Module	"RenderModule"
#define Video_Disable_OSXVSync	"DisableOSXVSync"
#define Video_Reset_OSXVSync_OnExit	"ResetOSXVSyncOnExit"
#define Video_AutoRemux	"AutoRemux"
#define Video_Base_CX	"BaseCX"
#define Video_Base_CY	"BaseCY"
#define Video_Output_CX	"OutputCX"
#define Video_Output_CY	"OutputCY"
#define Video_FPSType	"FPSType"
#define Video_FPSCommon	"FPSCommon"
#define Video_FPSInt	"FPSInt"
#define Video_FPSNum	"FPSNum"
#define Video_FPSDen	"FPSDen"
#define Video_ScaleType	"ScaleType"
#define Video_ColorFormat	"ColorFormat"
#define Video_ColorSpace	"ColorSpace"
#define Video_ColorRange	"ColorRange"


#define OutPut_Mode	"Mode"
#define OutPut_Reconnect	"Reconnect"
#define OutPut_RetryDelay	"RetryDelay"
#define OutPut_MaxRetry		"MaxRetries"
#define OutPut_LowLatencyEnable	"LowLatencyEnable"
#define OutPut_DelayEnable	"DelayEnable"
#define OutPut_DelaySec		"DelaySec"
#define OutPut_DelayPreserve	"DelayPreserve"
#define OutPut_FileNameFormatting	"FilenameFormatting"
#define OutPut_DynamicBitrate	"DynamicBitrate"

#define SimpleOutput_FilePath	"FilePath"
#define SimpleOutput_RecFormat	"RecFormat"
#define SimpleOutput_VBitrate	"VBitrate"
#define SimpleOutput_ABitrate	"VBitrage"
#define SimpleOutput_Use_Advanced	"UseAdvanced"
#define SimpleOutput_Preset	"Preset"
#define SimpleOutput_NVENC_Preset	"NVENCPreset"
#define SimpleOutput_Rec_Quality	"RecQuality"
#define SimpleOutput_Rec_RB	"RecRB"
#define SimpleOutput_Rec_RBTime	"RecRBTime"
#define SimpleOutput_Rec_RBSize	"RecRBSize"
#define SimpleOutput_Rec_RBPrefix	"RecRBPrefix"
#define SimpleOutput_StreamEncoder	"StreamEncoder"
#define SimpleOutput_RecEncoder		"RecEncoder"



#define Simple_Encoder_X264 "x264"
#define Simple_Encoder_X264_LowCPU "x264_lowcpu"
#define Simple_Encoder_QSV "qsv"
#define Simple_Encoder_NVENC "nvenc"
#define Simple_Encoder_AMD "amd"



class GeneralConfig {
public:
	static GeneralConfig& GetInstance();

	~GeneralConfig();

	//获取整个应用参数配置
	obs_data_t* GetApplicationConfig() const{
		return appConfig;
	}

	//获取基本参数配置
	obs_data_t* GetBasicConfig(){
		return obs_data_get_obj(appConfig,"basic");
	}

	//获取视频参数配置
	obs_data_t* GetVideoConfig() {
		return obs_data_get_obj(appConfig,"video");
	}

	//获取音频参数配置
	obs_data_t* GetAudioConfig(){
		return obs_data_get_obj(appConfig,"audio");
	}

	//获取输出流参数配置
	obs_data_t* GetOutputConfig(){
		return obs_data_get_obj(appConfig,"output");
	}

	//获取简单模式流配置
	obs_data_t* GetSimpleModeConfig(){
		obs_data_t* outputData = GetOutputConfig();
		return obs_data_get_obj(outputData,"simple");
	}

private:
	GeneralConfig();

	GeneralConfig(GeneralConfig&) = delete;
	GeneralConfig& operator=(const GeneralConfig&) = delete;

	//初始化默认配置
	void InitDefaultConfig();

	//从json文件中读取配置，并且覆盖默认的配置。这样可以保证可以保存用户配置数据
	void LoadFromJsonFile();

	//初始化基础参数配置
	void InitBasicConfig();

	//初始化音频参数配置
	void InitAudioConfig();

	//初始化视频参数配置
	void InitVideoConfig();

	//初始化输出流参数配置
	void InitOutputConfig();

	//初始化默认Simple简单模式输出流参数配置
	void InitSimpleOutputConfig();

	//简单模式下编码器为x264的回滚选择
	void CheckForSimpleModeX264Fallback();

private:
	//应用配置
	OBSData appConfig = nullptr;


};

#endif //OBS_STUDIO_GENERAL_CONFIG_H
