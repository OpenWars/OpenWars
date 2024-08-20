/*    
   ___                __        __
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

		AVFrame *av_frame = nullptr;
		AVFrame *av_frame_rgb32 = nullptr;
		SwsContext *sws_ctx = nullptr;
		u8 *frame_buff = nullptr;

		MediaPlayer::video_frame_t frame;
	} i_mp_ptr_t;

	i8 MediaPlayer::open(const char *filepath, const char *err) {
		i_mp_ptr_t *mp = valloc<i_mp_ptr_t>(err);
		if(mp == nullptr)
			return -1;

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

			mp->av_frame = av_frame_alloc();
			if(mp->av_frame == nullptr) {
				close();

				return Error {
					"Couldn't allocate enough memory for a video frame"
				};
			}

			mp->av_frame_rgb32 = av_frame_alloc();
			if(mp->av_frame_rgb32 == nullptr) {
				close();
				
				return Error {
					"Couldn't allocate enough memory for a converted video frame"
				};
			}

			// SWS_FAST_BILINEAR, SWS_BILINEAR, SWS_CUBIC, SWS_X,
			// SWS_POINT, SWS_AREA, SWS_BICUBLIN, SWS_GAUSS, SWS_SINC,
			// SWS_LANCZOS, SWS_SPLINE

			mp->sws_ctx = sws_getContext(codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height, AV_PIX_FMT_RGBA, SWS_POINT, nullptr, nullptr, nullptr);

			if(mp->sws_ctx == nullptr) {
				close();
				return Error { "Couldn't create a SWS context" };
			}

			u64 fb_len = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codec_ctx->width, codec_ctx->height, true);

			mp->frame_buff = (u8 *)av_malloc(fb_len);

			if(mp->frame_buff == nullptr) {
				close();
				return Error {
					"Couldn't allocate enough memory for the video framebuffer"
				};
			}

			av_image_fill_arrays(mp->av_frame_rgb32->data, mp->av_frame_rgb32->linesize, mp->frame_buff, AV_PIX_FMT_RGBA, codec_ctx->width, codec_ctx->height, true);

			mp->frame.width = codec_ctx->width;
			mp->frame.height = codec_ctx->height;
			mp->frame.data = mp->frame_buff;
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
		if(mp->fmt_ctx != nullptr)
			avformat_close_input(&mp->fmt_ctx);
		if(mp->av_frame != nullptr)
			av_frame_free(&mp->av_frame);
		if(mp->av_frame_rgb32 != nullptr)
			av_frame_free(&mp->av_frame_rgb32);
		if(mp->frame_buff != nullptr)
			av_free(mp->frame_buff);

		sws_freeContext(mp->sws_ctx);

		mp->codec_ctx_video = nullptr;
		mp->codec_ctx_audio = nullptr;
		mp->fmt_ctx = nullptr;
		mp->av_frame = nullptr;
		mp->av_frame_rgb32 = nullptr;
		mp->sws_ctx = nullptr;

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
		if(i_ptr == (uintptr_t)nullptr)
			return Error { "There is no media" };
		
		i_mp_ptr_t *mp = (i_mp_ptr_t *)i_ptr;

		int e;

		AVPacket packet;
		while((e = av_read_frame(mp->fmt_ctx, &packet)) >= 0) {
			if(packet.stream_index == mp->str_video->index) {
				e = avcodec_send_packet(mp->codec_ctx_video, &packet);
				if(e < 0) {
					close();

					if(av_strerror(e, err_buff, 128) == 0)
						return Error { err_buff };

					return Error { "Couldn't send next frame request" };
				}

				while((e = avcodec_receive_frame(mp->codec_ctx_video, mp->av_frame)) == 0) {
					return Error { nullptr };

					av_packet_unref(&packet);
					return Error { nullptr };
				};
			}

			av_packet_unref(&packet);
		};

		return Error { nullptr };
	};

	ErrorOr<MediaPlayer::video_frame_t *> MediaPlayer::get_frame(void) {
		if(i_ptr == (uintptr_t)nullptr)
			return Error { "There is no media" };
		
		i_mp_ptr_t *mp = (i_mp_ptr_t *)i_ptr;

		sws_scale(mp->sws_ctx, mp->av_frame->data, mp->av_frame->linesize, 0, mp->codec_ctx_video->height, mp->av_frame_rgb32->data, mp->av_frame_rgb32->linesize);

		return (&mp->frame);
	};

	ErrorOr<MediaPlayer::audio_samples_t *> MediaPlayer::get_samples(void) {
		// [TODO]
	};
};

#endif
