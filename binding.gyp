{
	"targets" : [
		{
			"target_name" : "node-espeak",
			"dependencies" : [
				"deps/binding.gyp:espeak"
			],
			"cflags" : [
				"-Wno-unused-parameter",
				"-Wno-missing-field-initializers",
				"-Wextra"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			],
			"sources" : [
				"src/node-espeak.cc"
			]
		}
	]
}
