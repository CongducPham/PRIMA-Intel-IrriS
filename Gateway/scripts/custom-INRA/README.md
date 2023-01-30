Guillaume's scripts custom INRA, to extract values and format them to CSV
=========================================================================

custom-INRA/extract_values.sh
-----------------------------
Extract values from the gateway.
Produces a text file, extracted.json, containing JSON blocks for each device identified by a semicolon (:) separated text line

custom-INRA/read_values_columns.py
----------------------------------
Converts extracted.json to another text file extracted_2.csv using comma separated values in rows.


# TODO:
-------
- add other sensor types?
