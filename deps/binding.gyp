{
	"target_defaults" : {
		"default_configuration" : "Debug",
		"configuration" : {
			"Debug" : {
				"defines" : ["DEBUG", "_DEBUG"]
			},
			"Release" : {
				"defines" : ["NODEBUG"]
			}
		}
	},
	"targets" : [
		{
			"target_name" : "espeak",
			"type" : "static_library",
			"cflags" : [
				"-Wno-unused-parameter",
				"-Wno-missing-field-initializers",
				"-Wextra",
				"-Wno-narrowing"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			],
			"sources" : [
				#"espeak/src/compiledata.cpp",
				"espeak/src/fifo.cpp",
				"espeak/src/speak.cpp",
				#"espeak/src/spectseq.cpp",
				"espeak/src/synth_mbrola.cpp",
				#"espeak/src/voicedlg.cpp",
				"espeak/src/compiledict.cpp",
				"espeak/src/espeak.cpp",
				"espeak/src/mbrowrap.cpp",
				"espeak/src/readclause.cpp",
				"espeak/src/speak_lib.cpp",
				"espeak/src/translate.cpp",
				"espeak/src/wave_pulse.cpp",
				"espeak/src/debug.cpp",
				#"espeak/src/espeakedit.cpp",
				#"espeak/src/formantdlg.cpp",
				"espeak/src/phonemelist.cpp",
				"espeak/src/setlengths.cpp",
				"espeak/src/voices.cpp",
				"espeak/src/wave_sada.cpp",
				"espeak/src/event.cpp",
				"espeak/src/intonation.cpp",
				#"espeak/src/menus.cpp",
				#"espeak/src/spect.cpp",
				"espeak/src/synthdata.cpp",
				#"espeak/src/transldlg.cpp",
				#"espeak/src/vowelchart.cpp",
				"espeak/src/dictionary.cpp",
				"espeak/src/klatt.cpp",
				"espeak/src/numbers.cpp",
				"espeak/src/sonic.cpp",
				#"espeak/src/spectdisplay.cpp",
				"espeak/src/synthesize.cpp",
				"espeak/src/tr_languages.cpp",
				"espeak/src/wave.cpp",
				"espeak/src/espeak_command.cpp",
				#"espeak/src/extras.cpp",
				#"espeak/src/options.cpp",
				#"espeak/src/prosodydisplay.cpp",
				"espeak/src/wavegen.cpp"
				#"src/node-espeak.cc"
			]
		}
	]
}
