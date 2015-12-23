#include <nan.h>
#include <v8.h>
#include <stdlib.h>
#include <iostream>

#define gender_none 0
#define gender_male 1
#define gender_female 2

#include "../deps/espeak/src/speak_lib.h"

using namespace node;
using namespace v8;

static int samplerate;
static Nan::Callback *callback;
static bool hasInit = false;
static bool hasCallback;
static unsigned int flags;

static int variant = 0;
static char *language = "en\0";
static char gender = gender_none;
static int age = 30;

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
			Local<Object> buffer = Nan::CopyBuffer(reinterpret_cast<char*>(wav), numSamples*2).ToLocalChecked();
			//Local<Object> buffer = Nan::NewBuffer(0).ToLocalChecked();
			Local<Value> argv[argc] = { buffer, Nan::New<Number>(numSamples), Nan::New<Number>(samplerate) };
			callback->Call(argc, argv);
		}
	}
	return 0;
}

static void reloadVoice() {
	espeak_VOICE *voice = new espeak_VOICE();
	voice->gender = gender;
	voice->age = age;
	voice->variant = variant;
	voice->languages = language;
	espeak_SetVoiceByProperties(voice);
}

static void GetVoice(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Local<Object> obj = Nan::New<Object>();
	if(gender == gender_none) {
		obj->Set(Nan::New("gender").ToLocalChecked(), Nan::New("none").ToLocalChecked());
	}
	else if(gender == gender_male) {
		obj->Set(Nan::New("gender").ToLocalChecked(), Nan::New("male").ToLocalChecked());
	}
	else if(gender == gender_female) {
		obj->Set(Nan::New("gender").ToLocalChecked(), Nan::New("female").ToLocalChecked());
	}
	obj->Set(Nan::New("variant").ToLocalChecked(), Nan::New(variant));
	obj->Set(Nan::New("age").ToLocalChecked(), Nan::New(age));
	obj->Set(Nan::New("language").ToLocalChecked(), Nan::New(std::string(language)).ToLocalChecked());
	info.GetReturnValue().Set(obj);
}

static void GetProperties(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Local<Object> obj = Nan::New<Object>();
	obj->Set(Nan::New("pitch").ToLocalChecked(), Nan::New(espeak_GetParameter(espeakPITCH, 1)));
	obj->Set(Nan::New("rate").ToLocalChecked(), Nan::New(espeak_GetParameter(espeakRATE, 1)));
	obj->Set(Nan::New("volume").ToLocalChecked(), Nan::New(espeak_GetParameter(espeakVOLUME, 1)));
	obj->Set(Nan::New("gap").ToLocalChecked(), Nan::New(espeak_GetParameter(espeakWORDGAP, 1)));
	obj->Set(Nan::New("range").ToLocalChecked(), Nan::New(espeak_GetParameter(espeakRANGE, 1)));
	info.GetReturnValue().Set(obj);
}

static void SetGender(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	gender = (char)num;
	reloadVoice();
}

static void SetAge(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	age = (char)num;
	reloadVoice();
}

static void SetVariant(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	variant = (char)num;
	reloadVoice();
}

static void SetRate(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	if(num < 80 || num > 450) {
		Nan::ThrowTypeError("Invalid rate. Valid values are integers in range 80 to 450.");
	}
	else {
		espeak_SetParameter(espeakRATE, (int) num, 0);
	}
}

static void SetVolume(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	if(num < 0) {
		Nan::ThrowTypeError("Invalid volume. Valid values are integers greater than 0. Integer bigger than 200 are not recommended.");
	}
	else {
		espeak_SetParameter(espeakVOLUME, (int) num, 0);
	}
}

static void SetPitch(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	if(num < 0 || num > 100) {
		Nan::ThrowTypeError("Invalid pitch. Valid values are integer between 0 and 100. Default is 50.");
	}
	else {
		espeak_SetParameter(espeakPITCH, (int) num, 0);
	}
}

static void SetRange(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	if(num < 0 || num > 100) {
		Nan::ThrowTypeError("Invalid range. Valid values are integers between 0 and 100. Default is 50.");
	}
	else {
		espeak_SetParameter(espeakRANGE, (int) num, 0);
	}
}

static void SetGap(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	double num = info[0]->NumberValue();
	if(num < 0) {
		Nan::ThrowTypeError("Invalid gap. Valid values are integers greater than 0.");
	}
	else {
		espeak_SetParameter(espeakWORDGAP, (int) num, 0);
	}
}

static void SetLanguage(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	Nan::Utf8String str(info[0]);
	language = new char[strlen(*str) + 1];
	strcpy(language, *str);
	reloadVoice();
}

static void Initialize(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	flags = espeakCHARS_AUTO | espeakENDPAUSE;
	hasInit = true;
	if(!info[0]->IsUndefined()) {
		Nan::Utf8String path(info[0]);
		espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, *path, 0);
	}
	else {
		espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, NULL, 0);
	}
	espeak_SetSynthCallback(SynthCallback);
	espeak_SetVoiceByName("mb-lt1");
}

static void Speak(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if(!hasInit) {
		Nan::ThrowError("ESpeak was not initialized but speak() was called. Did you forget to call initialize()?");
	}
	else {
		Nan::Utf8String str(info[0]);
		char *text = *str;
		espeak_Synth(text, str.length(), 0,POS_CHARACTER,0, flags, NULL, NULL);
		espeak_Synchronize();
	}
}

static void OnVoice(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	callback = new Nan::Callback(info[0].As<Function>());
	hasCallback = true;
}

static void Cancel(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	espeak_Cancel();
}

static void InitESpeak(Handle<Object> exports) {
	exports->Set(Nan::New("initialize").ToLocalChecked(), Nan::New<FunctionTemplate>(Initialize)->GetFunction());
	exports->Set(Nan::New("speak").ToLocalChecked(), Nan::New<FunctionTemplate>(Speak)->GetFunction());
	exports->Set(Nan::New("onVoice").ToLocalChecked(), Nan::New<FunctionTemplate>(OnVoice)->GetFunction());

	exports->Set(Nan::New("setAge").ToLocalChecked(), Nan::New<FunctionTemplate>(SetAge)->GetFunction());
	exports->Set(Nan::New("setGender").ToLocalChecked(), Nan::New<FunctionTemplate>(SetGender)->GetFunction());
	exports->Set(Nan::New("setLanguage").ToLocalChecked(), Nan::New<FunctionTemplate>(SetLanguage)->GetFunction());
	exports->Set(Nan::New("setVariant").ToLocalChecked(), Nan::New<FunctionTemplate>(SetVariant)->GetFunction());

	exports->Set(Nan::New("setPitch").ToLocalChecked(), Nan::New<FunctionTemplate>(SetPitch)->GetFunction());
	exports->Set(Nan::New("setRange").ToLocalChecked(), Nan::New<FunctionTemplate>(SetRange)->GetFunction());
	exports->Set(Nan::New("setRate").ToLocalChecked(), Nan::New<FunctionTemplate>(SetRate)->GetFunction());
	exports->Set(Nan::New("setVolume").ToLocalChecked(), Nan::New<FunctionTemplate>(SetVolume)->GetFunction());
	exports->Set(Nan::New("setGap").ToLocalChecked(), Nan::New<FunctionTemplate>(SetGap)->GetFunction());

	exports->Set(Nan::New("cancel").ToLocalChecked(), Nan::New<FunctionTemplate>(Cancel)->GetFunction());

	exports->Set(Nan::New("getProperties").ToLocalChecked(), Nan::New<FunctionTemplate>(GetProperties)->GetFunction());
	exports->Set(Nan::New("getVoice").ToLocalChecked(), Nan::New<FunctionTemplate>(GetVoice)->GetFunction());
}

NODE_MODULE(node_espeak, InitESpeak);
