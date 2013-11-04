weather-beacon
==============

An arduino-based, ambient weather forecast device.

Using the principles of <a href="http://en.wikipedia.org/wiki/Ambient_device">ambient display</a>, a localized weather station displays tomorrow's weather forecast in a visually passive means. The arduino device polls a server which generates a text string (via a PHP page). The text string is then used by the arduino to turn on a colored LED which corresponds to tomorrow's weather.