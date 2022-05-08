//
// Created by 李文禹 on 2022/1/24.
// 性能分析器帮助类
//

#ifndef OBS_STUDIO_PROFILER_HELPER_H
#define OBS_STUDIO_PROFILER_HELPER_H

#include <mutex>
#include <util/bmem.h>
#include <util/dstr.hpp>
#include <util/platform.h>
#include <util/profiler.hpp>
#include <util/cf-parser.h>
#include <obs-config.h>
#include <obs.hpp>
#include <fstream>
#include <sstream>
#include <sstream>
#include <functional>

class ProfilerHelper{
public:
	//函数指针，用来释放profiler_snapshot资源
	static std::function<void(profiler_snapshot_t*)> SnapshotRelease;

	//性能分析器快照unique指针，可以自动释放资源
	using ProfilerSnapshot = std::unique_ptr<profiler_snapshot_t,
						 decltype(SnapshotRelease)>;

	//函数指针，用来释放profiler_name_store_t
	static std::function<void(profiler_name_store_t*)> ProfilerNameStoreRelease;

	using ProfilerNameStore = std::unique_ptr<profiler_name_store_t,decltype(ProfilerNameStoreRelease)>;

	//函数指针，用来释放性能分析器资源，也包含了保存性能统计数据的功能。
	static std::function<void(void*)> ProfilerFree;


public:
	//获取性能统计快照
	static ProfilerSnapshot GetSnapshot();

	//创建
	static ProfilerNameStore CreateNameStore();

	//保存性能统计快照数据，保存到csv文件
	static void SaveProfilerData(const ProfilerSnapshot &snap);

};

#endif //OBS_STUDIO_PROFILER_HELPER_H
