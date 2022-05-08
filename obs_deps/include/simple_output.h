//
// Created by 李文禹 on 2022/1/26.
//

#ifndef OBS_STUDIO_SIMPLE_OUTPUT_H
#define OBS_STUDIO_SIMPLE_OUTPUT_H

#include "basic_output_handler.h"

struct SimpleOutput :  BasicOutputHandler{
	OBSEncoder aacStreaming;
	OBSEncoder h264Streaming;
	OBSEncoder aacRecording;
	OBSEncoder aacArchive;
	OBSEncoder h264Recording;

	std::string aacRecEncID;
	std::string aacStreamEncID;
	std::string aacArchiveEncID;

	std::string videoEncoder;
	std::string videoQuality;
	bool usingRecordingPreset = false;
	bool recordingConfigured = false;
	bool ffmpegOutput = false;
	bool lowCPUx264 = false;

	SimpleOutput();

	int CalcCRF(int crf);

	void UpdateStreamingSettings_amd(obs_data_t *settings, int bitrate);
	void UpdateRecordingSettings_x264_crf(int crf);
	void UpdateRecordingSettings_qsv11(int crf);
	void UpdateRecordingSettings_nvenc(int cqp);
	void UpdateRecordingSettings_amd_cqp(int cqp);
	void UpdateRecordingSettings();
	void UpdateRecordingAudioSettings();
	virtual void Update() override;

	void SetupOutputs() override;
	int GetAudioBitrate() const;

	void LoadRecordingPreset_h264(const char *encoder);
	void LoadRecordingPreset_Lossless();
	void LoadRecordingPreset();

	void LoadStreamingPreset_h264(const char *encoder);

	void UpdateRecording();
	bool ConfigureRecording(bool useReplayBuffer);

	virtual bool SetupStreaming(obs_service_t *service) override;
	virtual bool StartStreaming(obs_service_t *service) override;
	virtual bool StartRecording() override;
	virtual bool StartReplayBuffer() override;
	virtual void StopStreaming(bool force) override;
	virtual void StopRecording(bool force) override;
	virtual void StopReplayBuffer(bool force) override;
	virtual bool StreamingActive() const override;
	virtual bool RecordingActive() const override;
	virtual bool ReplayBufferActive() const override;

};



#endif //OBS_STUDIO_SIMPLE_OUTPUT_H
