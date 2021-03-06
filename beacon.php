<?php

	ini_set('display_errors', '1');  /* Display any PHP errors */

	function retrieveWundergroundWeather() { /* Get JSON string from Wunderground */
		/* Replace API with your own; change state and city to suit your location */
		$wundergroundFullUrl = "http://api.wunderground.com/api/your_Wunderground_API_here/forecast10day/q/ca/oakland.json"; 
		$curlObject = curl_init();
		curl_setopt($curlObject,CURLOPT_URL,$wundergroundFullUrl);
		curl_setopt($curlObject,CURLOPT_HEADER,false);
		curl_setopt($curlObject,CURLOPT_RETURNTRANSFER,true);
		$returnWundergroundWeather = curl_exec($curlObject);
		curl_close($curlObject);
		return $returnWundergroundWeather;
	}

	$json_string = retrieveWundergroundWeather(); /* Parse the JSON string */
	$parsed_json = json_decode($json_string, true);
	$currentHigh = $parsed_json['forecast']['simpleforecast']['forecastday'][0]['high']['fahrenheit'];
	$forecastHigh = $parsed_json['forecast']['simpleforecast']['forecastday'][1]['high']['fahrenheit'];
	$forecastDescription = $parsed_json['forecast']['simpleforecast']['forecastday'][0]['conditions'];
	
	$differenceTemp = $currentHigh - $forecastHigh; /* Determine temperature differences between today and tomorrow */

	if (abs($differenceTemp) <= 5)
		$beaconTemp = "S"; /* Same */
	elseif ($differenceTemp > 5)
		$beaconTemp = "C"; /* Colder */
	else
		$beaconTemp = "W"; /* Warmer */
	
	$precipitation = array("Chance of Flurries", "Rain Showers", "Chance of Rain", "Chance of Freezing Rain", "Chance of Sleet", "Chance of Snow", "Chance of Thunderstorms", "Chance of a Thunderstorm", "Flurries", "Freezing Rain", "Rain", "Sleet", "Snow", "Thunderstorms", "Thunderstorm");

	if (in_array($forecastDescription, $precipitation)) { /* Determine if there will be precipitation tomorrow */
		$beaconTemp = strtolower($beaconTemp);
	}

	echo '<text>'.$beaconTemp.'</text>'; /* Display output: S=Same C=Cold W=Warm */

?>