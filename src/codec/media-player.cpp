#ifndef __openwars__codec__media_player__cpp__
#define __openwars__codec__media_player__cpp__

#include "media-player.hpp"

extern "C" {
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/error.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>
};

#include <new>

namespace OpenWars {
	typedef struct _i_mp_ptr_t {
		AVFormatContext	*fmt_ctx = nullptr;
		AVStream *str_audio = nullptr;
		AVStream *str_video = nullptr;
		AVCodecContext *codec_ctx_audio = nullptr;
		AVCodecContext *codec_ctx_video = nullptr;

		// [TODO]
		AVFrame *av_frame = nullptr;
	} i_mp_ptr_t;

	ErrorOr<void> MediaPlayer::open(const char *filepath) {
		i_mp_ptr_t *mp;

		try {
			mp = new i_mp_ptr_t;
		} catch(std::bad_alloc &e) {
			return Error {
				"Couldn't allocate enough memory for a video context"
			};
		};

		i_ptr = (uintptr_t)mp;

		int e = avformat_open_input(&mp->fmt_ctx, filepath, nullptr, nullptr);
		if(e < 0) {
			if(av_strerror(e, err_buff, 128) == 0)
				return Error { err_buff };

			return Error { "Couldn't open the provided video file path" };
		}

		e = avformat_find_stream_info(mp->fmt_ctx, nullptr);
		if(e < 0) {
			if(av_strerror(e, err_buff, 128) == 0)
				return Error { err_buff };

			return Error { "Couldn't find a stream" };
		}

		AVStream *stream;
		for(unsigned int i = 0; i < mp->fmt_ctx->nb_streams; i++) {
			stream = mp->fmt_ctx->streams[i];

			if(avcodec_find_decoder(stream->codecpar->codec_id) != NULL) {
				if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
					mp->str_video = stream;

				if(stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
					mp->str_audio = stream;
			}
		};

		if(mp->str_video != nullptr) {
			AVCodec *codec = avcodec_find_decoder(mp->str_video->codecpar->codec_id);

			AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
			if(codec_ctx == nullptr) {
				close();
				return Error {
					"Couldn't allocate enough memory for a video context"
				};
			}

			mp->codec_ctx_video = codec_ctx;

			e = avcodec_parameters_to_context(codec_ctx, mp->str_video->codecpar);
			if(e < 0) {
				close();

				if(av_strerror(e, err_buff, 128) == 0)
					return Error { err_buff };

				return Error { "Couldn't copy video stream context" };
			}

			e = avcodec_open2(codec_ctx, codec, nullptr);
			if(e < 0) {
				close();

				if(av_strerror(e, err_buff, 128) == 0)
					return Error { err_buff };

				return Error { "Couldn't open video stream context" };
			}
		}


		if(mp->str_audio != nullptr) {
			AVCodec *codec = avcodec_find_decoder(mp->str_audio->codecpar->codec_id);

			AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
			if(codec_ctx == nullptr) {
				close();
				return Error {
					"Couldn't allocate enough memory for an audio context"
				};
			}

			mp->codec_ctx_audio = codec_ctx;

			e = avcodec_parameters_to_context(codec_ctx, mp->str_audio->codecpar);
			if(e < 0) {
				close();

				if(av_strerror(e, err_buff, 128) == 0)
					return Error { err_buff };

				return Error { "Couldn't copy audio stream context" };
			}

			e = avcodec_open2(codec_ctx, codec, nullptr);
			if(e < 0) {
				close();

				if(av_strerror(e, err_buff, 128) == 0)
					return Error { err_buff };

				return Error { "Couldn't open audio stream context" };
			}
		}

		return Error { nullptr };
	};

	void MediaPlayer::close(void) {
		if(i_ptr == (uintptr_t)nullptr)
			return;

		i_mp_ptr_t *mp = (i_mp_ptr_t *)i_ptr;

		if(mp->codec_ctx_video != nullptr)
			avcodec_free_context(&mp->codec_ctx_video);
		if(mp->codec_ctx_audio != nullptr)
			avcodec_free_context(&mp->codec_ctx_audio);

		avformat_free_context(mp->fmt_ctx);
		mp->fmt_ctx = nullptr;

		delete mp;
		i_ptr = (uintptr_t)nullptr;
	};

	MediaPlayer::time_t MediaPlayer::time(void) {
		// [TODO]
	};
	
	void MediaPlayer::time(MediaPlayer::time_t t) {
		// [TODO]
	};

	void MediaPlayer::pause(void) {
		// [TODO]
	};

	void MediaPlayer::resume(void) {
		// [TODO]
	};

	ErrorOr<void> MediaPlayer::process(void) {
		// [TODO]
	};

	ErrorOr<MediaPlayer::video_frame_t *> MediaPlayer::get_frame(void) {
		// [TODO]
	};

	ErrorOr<MediaPlayer::audio_samples_t *> MediaPlayer::get_samples(void) {
		// [TODO]
	};
};

#endif
