Efficient Sequential Pattern Mining Algorithms (C++) Version 1.0 - Memory-Efficient Sequential Pattern Mining with Hybrid Tries

Copyright (C) 2025 Amin Hosseininasab<br/>
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

DESCRIPTION<br/>
    Provided algorithms are C++ implementations of memory-efficient sequential pattern mining. 

SISTER REPOSITORY<br/>
    A Python implementation of the codes (Python wrapper with C++ core) is under development and will be made available when complete

ALGORITHMS<br/>
    HTMiner: Hybrid trie based model (BD trie + vector model of the dataset).<br/>
    Large HTMiner: Large version of HTMiner, intended for datasets with over 4,294,967,295 non overlapping sequences (integer limit of C++).<br/>
    BDMiner: Binary trie based model (intended for shorter sequences).<br/>
    Large BDMiner: Large version of BDMiner, intended for datasets with over 4,294,967,295 non overlapping sequences.<br/>
    PrefixProjection: Effective (memory + time) implementation of Prefix Projection algorithm.<br/>
    Large HTMiner: Large version of Prefix Projection, intended for datasets with over 4,294,967,295 sequences.<br/>
    	
REFERENCE<br/>
    Hosseininasab, A., van Hoeve, W.J. and Cire, A.A., 2024. 
    Memory-efficient sequential pattern mining with hybrid tries. 
    Journal of Machine Learning Research 25(227), pp.1-29.

CONTACT<br/>
    a.hosseininasab@ufl.edu

INSTALLATION<br/>
    Designed for Linux systems. Installation from Makefile. Simply use make. Output is algorithm-specific executable.

DOWNLOAD<br/>
    Algorithms can be downloaded from https://github.com/aminhn/HTMiner

COMMAND LINE USAGE<br/>
    Example: ./HTMiner -folder ./data/ -file kosarak.txt -thr 0.001 -npre -time 3600 -dic -out kosarak_patt.txt

COMMAND LINE COMMANDS<br/>
    -folder     	path containing input database file
	-file		    input file name
	-thr		    frequency threshold in mining algorithm (double precision). If 0 < thr < 1, then thr*N is used as the minumum support, where N is the total number of sequences in the input file. If thr >= 1, then thr is used as the minimum support directly.
	-npre		    no preprocessing of input file (apriori removes non-frequent items) 
    -time       	time limit (in seconds)
 	-dic		    use if input databse events are not represented by consecutive numerical values (will encounter an error if not used when required. May be used by default)
	-out		    display found patterns in terminal
    -out file   	output found patterns to "file" (replace "file" with intended output file name)
    -out+file   	display found patterns to terminal and also output them to "file"  

INPUT FILE<br/> 
    Input databse consists of rows of sequences, where each sequence is an ordered set of events denoted by numericals or literals.<br/> 	
    Negative events indicate the start of an itemset. Positive events fall within the same itemset as their previous negative event. Itemsets must be organized in increasing values. If itemsets are not required, sequences should be represented by all negative events.<br/>
	Example -1 2 3 -2 -1 3 4 is equivalent to {1 2 3} {2} {1 3 4}, or commonly represented by seperators "-1" -1 1 2 3 -1 2 -1 1 3 4.<br/>
	Example -1 -3 -2 -1 -5 represents a sequence of 1 3 2 1 5 with no itemsets.<br/>

OUTPUT FILE<br/>
    Ouput file, if either -out file or out+file commanda RE used, gives rows of patterns with associated frequency. Frequency of a pattern is the number of sequences 
    in the input database that contain that pattern and satisfy all defined constraints on their associated attribute values.
