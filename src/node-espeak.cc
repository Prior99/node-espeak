#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>
#include <nan.h>
#include <string.h>
#include <iostream>

#include "../deps/espeak/src/speak_lib.h"

using namespace node;
using namespace v8;

class ESpeak : public ObjectWrap {
	private:
		static Persistent<Function> constructor;
	public:
		ESpeak() {
			espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0);
		}

		static NAN_METHOD(Speak) {
			NanScope();
			std::cout << "Hello" << std::endl;
			char text[32] = {"1. Labas. Šauk. Laikas. Taika"};
			unsigned int size = 0;
			espeak_SetVoiceByName("mb-lt1");
			while(text[size]!='\0') size++;
			unsigned int flags=espeakCHARS_AUTO | espeakENDPAUSE;
			espeak_Synth(text, size+1, 0,POS_CHARACTER,0, flags, NULL, NULL);
			espeak_Synchronize();
			NanReturnUndefined();
		}

		~ESpeak() {
			espeak_Synchronize();
		}

		static void Init(Handle<Object> exports) {
			Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
			tpl->SetClassName(NanNew("ESpeak"));
			tpl->InstanceTemplate()->SetInternalFieldCount(1);

			NODE_SET_PROTOTYPE_METHOD(tpl, "speak", Speak);

			exports->Set(NanNew("ESpeak"), tpl->GetFunction());
		}

		static NAN_METHOD(New) {
			NanScope();
			if(args.IsConstructCall()) {
				ESpeak* espeak = new ESpeak();
				espeak->Wrap(args.This());
				NanReturnValue(args.This());
			}
			else {
				NanReturnUndefined();
			}
		}
};

void InitESpeak(Handle<Object> exports) {
	ESpeak::Init(exports);
}

NODE_MODULE(node_espeak, InitESpeak)
