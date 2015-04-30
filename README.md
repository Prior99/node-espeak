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

License
-------
This project is published under the terms of version 3 of the GNU General Public License.
