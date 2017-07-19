# Searcher

A very specific searcher.

Input: Receives two command line parameters. The first parameter is a path to a file or folder, while the second parameter is the string to be searched for (maximum length of 128 characters).

Output: Prints the output to stdout lines in format `"<file>(<position>):<prefix>...<suffix>"`, where:

`<file>`: Name of a file containing the string
<br>
`<position>`: Offset within the file where the string started (zero based)
<br>
`<prefix>`: Three characters in front of the match
<br>
`<suffix>`: Three characters after the match


When there are not enough characters for the whole prefix or suffix (at the beginning or end of the file), it prints as many as there are available. In the prefixes and suffixes, tabs are printed as "\t" and new lines as "\n".

It assumes that the path can point to either a single text file, or to a directory with files and/or more directories; it recursively traverses and process all files in sub-directories. It takes into account that files might be very large (potential memory constraints/limitations).
