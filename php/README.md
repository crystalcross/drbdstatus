# drbdinfo.inc

** DRBDINFO.inc ** - is a quick php utility which leverages the power of the *drbdstatus* tool as well as *drbdsetup* to create an array of information about all of your drbd nodes.  

This array can easily be converted into a ***JSON*** file for use with a javascript library.  This script is intended to be included in your php program and called with no arguments.

/<?php
	
	include "drbdinfo.inc";

	$data = drbd_get_info();

	print_r($data);

?/>
