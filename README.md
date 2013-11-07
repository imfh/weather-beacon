weather-beacon
==============

An arduino-based, ambient weather forecast device.

With the increasing ubiquity of daily data, let’s reduce some of the sensory clutter into meaningful information. Using the principles of <a href="http://en.wikipedia.org/wiki/Ambient_device">ambient display</a>, the Ambient Weather Beacon is a visually unobtrusive, localized weather station displaying tomorrow’s weather forecast.

The Arduino device polls a server which generates a text string (via a PHP page, using data from a JSON source). The text string is then used by the Arduino to turn on a colored LED which corresponds to tomorrow’s weather.

The Ambient Weather Beacon filters the data and simplifies the forecast:

<em>Blue</em> = significantly colder

<em>Red</em> = significantly warmer

<em>White</em> = same as today

<em>Pulsing</em> = precipitation tomorrow

<img src="http://fh.io/wp-content/uploads/beacon-banner.jpg">

This project is a physical object based on the site <a href="http://wthr.im">wthr.im</a> and more information may be found on the <a href="http://fh.io/ambient-weather-beacon/">Ambient Weather Beacon site</a>.