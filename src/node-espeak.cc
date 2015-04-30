#include <v8.h>
#include <nan.h>

#include "../deps/espeak/src/speak_lib.h"

using namespace node;
using namespace v8;

static int samplerate;
static NanCallback *callback;
static bool hasInit = false;
static bool hasCallback;
static unsigned int flags;

static int SynthCallback(short *wav, int numSamples, espeak_EVENT *events) {
	while(events->type != 0) {
		if(events->type == espeakEVENT_SAMPLERATE) {
			samplerate = events->id.number;
		}
		events++;
	}
	if(numSamples > 0) {
		if(hasCallback) {
			const unsigned argc = 3;
			Local<Object> buffer = NanNewBufferHandle(reinterpret_cast<char*>(wav), numSamples*2);
			Local<Value> argv[argc] = { buffer, NanNew<Number>(numSamples), NanNew<Number>(samplerate) };
			callback->Call(argc, argv);
		}
	}
	return 0;
}

static NAN_METHOD(Initialize) {
	NanScope();
	flags = espeakCHARS_AUTO | espeakENDPAUSE;
	hasInit = true;
	espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, NULL, 0);
	espeak_SetSynthCallback(SynthCallback);
	espeak_SetVoiceByName("mb-lt1");
	NanReturnUndefined();
}

static NAN_METHOD(Speak) {
	NanScope();
	if(!hasInit) {
		NanThrowError("ESpeak was not initialized but speak() was called. Did you forget to call initialize()?");
	}
	else {
		NanUtf8String str(args[0]);
		char *text = *str;
		espeak_Synth(text, str.length(), 0,POS_CHARACTER,0, flags, NULL, NULL);
		espeak_Synchronize();
	}
	NanReturnUndefined();
}

static NAN_METHOD(OnVoice) {
	NanScope();
	callback = new NanCallback(args[0].As<Function>());
	hasCallback = true;
	NanReturnUndefined();
}

static void InitESpeak(Handle<Object> exports) {
	exports->Set(NanNew("initialize"), NanNew<FunctionTemplate>(Initialize)->GetFunction());
	exports->Set(NanNew("speak"), NanNew<FunctionTemplate>(Speak)->GetFunction());
	exports->Set(NanNew("onVoice"), NanNew<FunctionTemplate>(OnVoice)->GetFunction());
}

NODE_MODULE(node_espeak, InitESpeak);
