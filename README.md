eSpeak for node
===============

This is a native binding of eSpeak for node.
eSpeak is a compact open source software speech synthesizer for English and other languages.

Usage
-----

Install the package with

	npm install node-espeak

And then use it like this:

```javascript
var ESpeak = require('node-espeak');
ESpeak.initialize();
ESpeak.onVoice(function(wav, samples, samplerate) {
	// TODO: Do something useful
});
ESpeak.speak("Hello world!");
```

API
---

### initialize([voice, [properties]])

Must be called before eSpeak can be used, other wise an error will be thrown.

You may specify an object containing options for the voice to use. This will be passed to the ```setVoice()``` method.

You may also specify an object containing additional properties which will be passed to ```setProperties()```.

### speak(text)

Will synthesize the given text. The synthesized audio will be passed to the function you specified with the ```onVoice()``` method.

### onVoice(callback)

Will set the callback to call whenever speech was synthesized.

The callback should have three parameters:

* **data**: The raw audio samples.
* **samplerate**: The sample rate.
* **samples**: The amount of samples generated.

### getVoice()

Returns an object containing information about the voice eSpeak is using.
The object will contain the following properties:

* **gender**: "male", "female" or "none".
* **language**: The two letter code of the language currently used.
* **age**: The age of the voice.
* **variant**: The variant of the voice eSpeak is using.

### getProperties()

Returns an object containing the current properties of eSpeak.
The object will contain the following properties:

* **pitch**: The pitch. Values from 0 to 100.
* **volume**: The current volume. Values from 0 to 200 or higher.
* **range**: The range. 0 is totally monotonous, default is 50 and maximum is 100.
* **rate**: The speed the voice will be played back in words per minute. Valus from 80 to 450.
* **gap**: The gap between the words in 10ms (at normal rate).

### setProperties(properties)

Will set the additional properties. This method takes an object as argument in the form of the object specified in ```getProperties()```.
You may also only specify some of the properties.


### setVoice(voice)

Sets the voice to use. See the object defined in ```getVoice()``` for additional information about the object to pass to this method.
You may also only specify some of the properties.


You can also set each property and option for the voice with one of the following seperate setters:

* setPitch(pitch)
* setVolume(volume)
* setRange(range)
* setRate(rate)
* setGap(gap)
* setAge(age)
* setLanguage(language)
* setVariant(variant)
* setGender(gender)

License
-------
This project is published under the terms of version 3 of the GNU General Public License.
