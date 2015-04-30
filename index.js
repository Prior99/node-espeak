var ESpeak = require('bindings')('node-espeak');

function genderToNumber(gender) {
	gender = gender.toLowerCase();
	if(gender === "male" || gender === "m" || gender === 1) {
		return 1;
	}
	else if(gender === "female" || gender === "f" || gender === 2) {
		return 2;
	}
	else if(gender === "none" || gender === "default" || gender === 0) {
		return 0;
	}
	else {
		throw TypeError("Allowed genders are: male, female and none.");
	}
}

module.exports = {
	initialize : function(options) {
		ESpeak.initialize();
		if(options !== undefined) {
			this.setVoice(options);
		}
	},
	setVoice : function(voice) {
		if(typeof voice === "object") {
			voice.language = voice.lang;
			if(voice.age !== undefined) {
				this.setAge(voice.age);
			}
			if(voice.gender !== undefined) {
				this.setGender(voice.gender);
			}
			if(voice.language !== undefined) {
				this.setLanguage(voice.language);
			}
			if(voice.variant !== undefined) {
				this.setVariant(voice.variant);
			}
		}
		else {
			throw new TypeError("Set the voice via a name or an object containing it's properties.");
		}
	},
	setAge : function(age) {
		ESpeak.setAge(age);
	},
	setGender : function(gender) {
		ESpeak.setGender(genderToNumber(gender));
	},
	setLanguage : function(lang) {
		ESpeak.setLanguage(lang);
	},
	setVariant : function(variant) {
		ESpeak.setVariant(variant);
	},
	getVoice : ESpeak.getVoice,
	onVoice : ESpeak.onVoice,
	speak : ESpeak.speak
};
