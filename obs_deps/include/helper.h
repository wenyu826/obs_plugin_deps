//
// Created by 李文禹 on 2022/1/24.
//

#ifndef OBS_STUDIO_HELPER_H
#define OBS_STUDIO_HELPER_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include <fstream>
#include <util/bmem.h>
#include <util/dstr.hpp>
#include <util/platform.h>
#include <util/profiler.hpp>
#include <util/cf-parser.h>
#include <obs-config.h>
#include <obs.hpp>
#include <graphics/graphics.h>
#include <graphics/vec4.h>
#include <graphics/matrix4.h>


#define Application_Name	"live"

#define Outline_Color	0xffd0d0d0
#define Line_Length	0.1f

// Rec. ITU-R BT.1848-1 / EBU R 95
#define Action_Safe_Percent 0.035f       // 3.5%
#define Graphics_Safe_Percent 0.05f      // 5.0%
#define FourByThree_Safe_Percent 0.1625f // 16.25%

#ifdef __APPLE__
#define Input_Audio_Source  "coreaudio_input_capture"
#define Output_Audio_Source "coreaudio_output_capture"
#elif _WIN32
#define Input_Audio_Source  "wasapi_input_capture"
#define Output_Audio_Source "wasapi_output_capture"
#else
#define Input_Audio_Source  "pulse_input_capture"
#define Output_Audio_Source "pulse_output_capture"
#endif

class Helper{
public:
	//最多保存的日志文件数量
	static const unsigned  int maxLogs = 10;

	//目录最新的日志文件名
	static std::string lastlogFile;

	//最新的crash日志文件
	static std::string lastCrashLogFile;

	//当前使用的日志文件
	static std::string currentlogFile;

	//日志处理
	static log_handler_t defaultLogHandler;

	//性能分析器
	static profiler_name_store_t* globalProfilerNameStore;

	static std::once_flag populateBitrateMap;

	static std::map<int, const char *> bitrateMap;

#ifdef _WIN32
	static void LoadDebugPrivilege();

	static void MainCrashHander(const char* format, va_list args, void* param);

#endif

	//如果超过了最大日志文件数，则删除日志目录最老的一个日志文件
	static void DeleteOldestFile(bool hasPrefix,const char *location);

	//获取目录下最新的日志文件
	static void GetLastLog(bool has_prefix, const char *subdir_to_use,
			       std::string &last);

	//根据时间日期，生成日志文件名
	static std::string GenerateTimeDateFilename(const char *extension, bool noSpace = true);

	//日志处理函数
	static void DoLog(int log_level, const char *msg, va_list args, void *param);

	static bool TooManyRepeatedEntries(std::fstream &logFile, const char *msg,
				    const char *output_str);

	static void LogStringChunk(std::fstream &logFile, char *str, int log_level);

	static void LogString(std::fstream &logFile, const char *timeString,
			      char *str, int log_level);

	static int SumChars(const char *str);

	static std::string CurrentTimeString();

	static std::string CurrentDateTimeString();

	//创建日志文件
	static void CreateLogFile(std::fstream &fileLog);

	//根据日志文件名称，计算时间戳。(文件格式 yyyy-mm-ddhh-mm-ss)
	static uint64_t CovertLogName(bool hasPrefix, const char *name);

	//获取配置文件的语言包列表
	static std::vector<std::pair<std::string, std::string>> GetLocaleNames();

	//获取程序数据目录
	static int GetProgramDataPath(char *path, size_t size, const char *name);

	static char *GetProgramDataPathPtr(const char *name);

	static char* GetConfigPathPtr(const char* name);

	static int GetConfigPath(char *path, size_t size, const char *name);

	//检查编码器是否可用
	static bool EncoderAvailable(const char *encoder);

	static std::string  GetFormatString(const char *format, const char *prefix,
						  const char *suffix);

	//获取录制输出文件名
	static std::string GetOutputFilename(const char *path, const char *ext, bool noSpace,
				 bool overwrite, const char *format);

	static std::string GenerateSpecifiedFilename(const char *extension, bool noSpace,
						     const char *format);

	//确保文件夹存在，如果不存在则创建
	static void EnsureDirectoryExists(std::string& path);

	static void FindBestFilename(std::string &strPath, bool noSpace);

	static bool CreateAACEncoder(OBSEncoder &res, std::string &id, int bitrate,
				     const char *name, size_t idx);

	static const char *GetAACEncoderForBitrate(int bitrate);

	static const std::map<int, const char *> &GetAACEncoderBitrateMap();

	static int FindClosestAvailableAACBitrate(int bitrate);

	static const char *FindAudioEncoderFromCodec(const char *type);

	//根据配置信息计算帧率
	static void GetConfigFPS(uint32_t &num, uint32_t &den);

	//根据配置参数获取gpu渲染模块,Opengl/d3d11
	static const char* GetRenderModule();

	static enum video_format GetVideoFormatFromName(const char* name);

	static enum obs_scale_type GetScaleType();

	static void InitSafeAreas(gs_vertbuffer_t **actionSafeMargin,
				  gs_vertbuffer_t **graphicsSafeMargin,
				  gs_vertbuffer_t **fourByThreeSafeMargin,
				  gs_vertbuffer_t **leftLine,
				  gs_vertbuffer_t **topLine,
				  gs_vertbuffer_t **rightLine);

	static void RenderSafeAreas(gs_vertbuffer_t *vb, int cx, int cy);

	static void GetCenterPosFromPixelScale(int baseCX, int baseCY,
					       int windowCX, int windowCY,
					       int &x, int &y, float scale);

	//根据渲染窗口大小和基础画布大小，计算窗口的缩放比例，以及窗口渲染的中心点位置，
	//保证窗口渲染区域比例和画布比例一致
	static void GetScaleAndCenterPos(int baseCX, int baseCY, int windowCX,
					 int windowCY, int &x, int &y,
					 float &scale);

	//检测系统是否有对应的音频设备
	static bool HasAudioDevices(const char* id);

	//重置音频设备信道,音频用使用6个信号 1-6
	// 1：桌面输出音频1
	// 2：桌面输出音频2
	// 3：麦克风输入1
	// 4: 麦克风输入2
	// 5：麦克风输入3
	// 6: 麦克风输入4
	//
	static void ResetAudioDevice(const char* sourceId, const char* deviceId,
				     const char* deviceDesc,int channel);

	//



};

#endif //OBS_STUDIO_HELPER_H
