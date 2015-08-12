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
	initialize : function(voice, properties, espeakData) {
		ESpeak.initialize(espeakData);
		if(voice !== undefined) {
			this.setVoice(voice);
		}
		if(properties !== undefined) {
			this.setProperties(properties);
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
	setProperties : function(prop) {
		if(typeof prop === "object") {
			if(prop.rate !== undefined) {
				this.setRate(prop.rate);
			}
			if(prop.volume !== undefined) {
				this.setVolume(prop.volume);
			}
			if(prop.range !== undefined) {
				this.setRange(prop.range);
			}
			if(prop.gap !== undefined) {
				this.setGap(prop.gap);
			}
			if(prop.pitch !== undefined) {
				this.setPitch(prop.pitch);
			}
		}
		else {
			throw new TypeError("Set the voices properties using an object.");
		}
	},
	setGender : function(gender) {
		ESpeak.setGender(genderToNumber(gender));
	},
	cancel : ESpeak.cancel,
	setRate : ESpeak.setRate,
	setVolume : ESpeak.setVolume,
	setRange : ESpeak.setRange,
	setGap : ESpeak.setGap,
	setPitch : ESpeak.setPitch,
	setAge : ESpeak.setAge,
	setLanguage : ESpeak.setLanguage,
	setVariant : ESpeak.setVariant,
	getProperties : ESpeak.getProperties,
	getVoice : ESpeak.getVoice,
	onVoice : ESpeak.onVoice,
	speak : ESpeak.speak
};
