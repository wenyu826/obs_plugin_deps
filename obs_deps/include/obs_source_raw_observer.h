#pragma once
#include <obs.hpp>
#include <util/dstr.h>
#include "obs_proxy.h"


class OBS_PROXY_EXPORT OBSSourceRawDataObserver
{
public:
	OBSSourceRawDataObserver(const char* name);
	~OBSSourceRawDataObserver();

	/* RGBA格式图像数据回调 */
	virtual void onRGBAVideoFrame(uint8_t* data, uint32_t lineSize,int width, int height) = 0;

	/* 开始视频数据抓取 */
	void startVideoObserver();

	/* 停止视频数据抓取 */
	void stopVideoObserver();

	/* 更新抓取的资源名 */
	void updateSourceName(const char* name);

protected:
	static void videoTick(void* param, float);

private:
	struct dstr sourceName;
	gs_texrender_t* texrender = nullptr;
	gs_stagesurf_t* stagesurf = nullptr;
	int cx;
	int cy;
	bool isMonitoring = false;
};

