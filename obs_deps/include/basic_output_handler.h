//
// Created by 李文禹 on 2022/1/25.
//

#ifndef OBS_STUDIO_BASIC_OUTPUT_HANDLER_H
#define OBS_STUDIO_BASIC_OUTPUT_HANDLER_H

#include <iostream>
#include <obs.hpp>

#define FTL_PROTOCOL "ftl"
#define RTMP_PROTOCOL "rtmp"


struct BasicOutputHandler {
	OBSOutput fileOutput;
	OBSOutput streamOutput;
	OBSOutput replayBuffer;
	bool streamingActive = false;
	bool recordingActive = false;
	bool delayActive = false;
	bool replayBufferActive = false;

	std::string outputType;
	std::string lastError;

	std::string lastRecordingPath;

	OBSSignal startRecording;
	OBSSignal stopRecording;
	OBSSignal startReplayBuffer;
	OBSSignal stopReplayBuffer;
	OBSSignal startStreaming;
	OBSSignal stopStreaming;
	OBSSignal streamDelayStarting;
	OBSSignal streamStopping;
	OBSSignal recordStopping;
	OBSSignal replayBufferStopping;
	OBSSignal replayBufferSaved;

	BasicOutputHandler();

	virtual ~BasicOutputHandler();

	virtual bool SetupStreaming(obs_service_t *service) = 0;
	virtual bool StartStreaming(obs_service_t *service) = 0;
	virtual bool StartRecording() = 0;
	virtual bool StartReplayBuffer() { return false; }
	virtual void StopStreaming(bool force = false) = 0;
	virtual void StopRecording(bool force = false) = 0;
	virtual void StopReplayBuffer(bool force = false) { (void)force; }
	virtual bool StreamingActive() const = 0;
	virtual bool RecordingActive() const = 0;
	virtual bool ReplayBufferActive() const { return false; }

	virtual void Update() = 0;
	virtual void SetupOutputs() = 0;

	inline bool Active() const
	{
		return streamingActive || recordingActive || delayActive ||
		       replayBufferActive;
	}

	static void OBSStreamStarting(void *data, calldata_t *params);
	static void OBSStreamStopping(void *data, calldata_t *params);
	static void OBSStartStreaming(void *data, calldata_t *params);
	static void OBSStopStreaming(void *data, calldata_t *params);
	static void OBSStartRecording(void *data, calldata_t *params);
	static void OBSStopRecording(void *data, calldata_t *params);
	static void OBSRecordStopping(void *data, calldata_t *params);
	static void OBSStartReplayBuffer(void *data, calldata_t *params);
	static void OBSStopReplayBuffer(void *data, calldata_t *params);
	static void OBSReplayBufferStopping(void *data, calldata_t *params);
	static void OBSReplayBufferSaved(void *data, calldata_t *);

protected:
	void SetupAutoRemux(const char *&ext);
	std::string GetRecordingFilename(const char *path, const char *ext,
					 bool noSpace, bool overwrite,
					 const char *format, bool ffmpeg);
};



BasicOutputHandler *CreateSimpleOutputHandler();
BasicOutputHandler *CreateAdvancedOutputHandler();



#endif //OBS_STUDIO_BASIC_OUTPUT_HANDLER_H
