Guillaume's scripts custom INRA, to extract values and format them to CSV
=========================================================================

custom-INRA/extract_values.sh
-----------------------------
Extract values from the gateway.
Produces a text file, extracted.json, containing JSON blocks for each device identified by a semicolon (:) separated text line

custom-INRA/read_values_columns.py
----------------------------------
Converts extracted.json to:
	- another text file extracted_2.csv using comma separated values in rows;
	- another text file extracted_Bondy_aggregated.csv using comma separated values in columns (one unified column of timestamps);
	- JSON;
	- plots (for the different sensor types in presence), but require matplotlib and pandas
		- achievable with an internet connexion and: 
			pip install pandas
			python -m pip install -U matplotlib


# TODO:
-------
- add other device/sensor types?
