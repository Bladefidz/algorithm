gensort Data Generator

The gensort program can be used to generate input records for the sort benchmarks presented on www.sortbenchmark.org. The program currently can be compiled on Linux or Windows systems. Sort benchmark contestants may elect to extract parts of the gensort code and run them as part of a distributed application. The latest version, 1.5, includes the ability to generate records with skewed key values.
Download

The current version of the sort generator and output data validator programs utilizes the SUMP Pump library, is multithreaded and performs direct and asynchronous i/o's by default. Source code for this version, including build instructions on Linux and Windows can be downloaded here: gensort-1.5.tar.gz

Linux gensort and valsort executables, both 32-bit and 64-bit, can be downloaded here: gensort-linux-1.5.tar.gz

Windows gensort and valsort executables, 32-bit only, can be downloaded here: gensort-win-1.5.zip

Source code for the prior versions of gensort and valsort, single-threaded and only regular i/o capable, can be downloaded here: gensort-1.4.tar.gz; gensort-1.2.tar.gz
Standard Documentation

If the gensort program is invoked with no arguments or does not recognize its command line parameters, it prints the following to standard error:

Gensort Sort Input Generator

usage: gensort [-a] [-c] [-bSTARTING_REC_NUM] [-tN] NUM_RECS FILE_NAME[,opts]
-a        Generate ascii records required for PennySort or JouleSort.
          These records are also an alternative input for the other
          sort benchmarks.  Without this flag, binary records will be
          generated that contain the highest density of randomness in
          the 10-byte key.
-c        Calculate the sum of the crc32 checksums of each of the
          generated records and send it to standard error.
-bN       Set the beginning record generated to N. By default the
          first record generated is record 0.
-s        Generate input records with skewed keys. If used with -a
          option, then skewed ascii records are generated.
-tN       Use N internal program threads to generate the records.
NUM_RECS  The number of sequential records to generate.
FILE_NAME[,opts] The name of the file to write the records to.
          File options may immediately follow the file name:
          ,buf           Use buffered and synchronous file writes,
                         instead of the default direct and asynchronous
                         writes.
          ,dir           Use direct and asynchronous file writes.
                         The is the default.
          ,trans=N[k,m,g] Sets the file write request size in bytes,
                         kilobytes, megabytes or gigabytes.
          ,count=N       Sets the maximum number of simultaneous
                         asynchronous write requests allowed.

Example 1 - to generate 1000000 ascii records starting at record 0 to
the file named "pennyinput":
    gensort -a 1000000 pennyinput

Example 2 - to generate 1000 binary records beginning with record 2000
to the file named "partition2":
    gensort -b2000 1000 partition2

Copyright (C) 2009 - 2011, Chris Nyberg

This program is free software; you can redistribute it and/or
modify it under the terms of Version 2 of the GNU General Public
License as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Version 1.5, cvs $Revision: 1.14 $
SUMP Pump version 1.2.3, svn: 130

Partitioned Input

Gensort can be used to generate separate input partitions. This allows multiple instances of gensort to be run in parallel to generate the sort benchmark input.

The following output demonstrates that data can either be generated in partitions (part0 and part1), or in one file (whole), and that the concatenation of the two partitions (caten) is the same as the whole:

$ gensort -b0 1000 part0
$ gensort -b1000 1000 part1
$ cat part0 part1 > caten
$ gensort -b0 2000 whole
$ cmp whole caten
$

valsort Output Validator

The valsort program can be used to validate the sort output file is correct. It performs the following tasks:

    Counts the number of records.
    Calculates the sum of the crc32 checksums for each record.
    Verifies the reoords are in sorted order.
    Counts the number of duplicate keys.

Standard Documentation

If the valsort program is invoked with no arguments or does not recognize its command line parameters, it prints the following to standard error:

Valsort Sort Output Validator

usage: valsort [-i] [-q] [-tN] [-o SUMMARY_FILE] [-s] FILE_NAME[,opts]
-i        Use case insensitive ascii comparisons (optional for PennySort).
          Case sensitive ascii or binary keys are assumed by default.
-q        Quiet mode, don't output diagnostic text.
-o SUMMARY_FILE  Output a summary of the validated records. This method
          can be used to validate partitioned sort outputs separately.
          The contents of the separate summary files can then be
          concatenated into a single file that can be checked using
          the valsort program with the -s flag.
-s        The file to validate contains partition summaries instead of
          sorted records.
-tN       Use N internal program threads to validate the records.
FILE_NAME[,opts] The name of the sort output file or the partition
          summaries file to validate. File options may immediately
          follow the file name:
          ,buf           Use buffered and synchronous file reads,
                         instead of the default direct and asynchronous
                         reads.
          ,dir           Use direct and asynchronous file reads.
                         The is the default.
          ,trans=N[k,m,g] Sets the file read request size in bytes,
                         kilobytes, megabytes or gigabytes.
          ,count=N       Sets the maximum number of simultaneous
                         asynchronous read requests allowed.

Example 1 - to validate the sorted order of a single sort output file:
    valsort sortoutputfile

Example 2 - to validate the sorted order of output that has been
partitioned into 4 output files: out0.dat, out1.dat, out2.dat and out3.dat:
    valsort -o out0.sum out0.dat
    valsort -o out1.sum out1.dat
    valsort -o out2.sum out2.dat
    valsort -o out3.sum out3.dat
    cat out0.sum out1.sum out2.sum out3.sum > all.sum
    valsort -s all.sum

Copyright (C) 2009 - 2011, Chris Nyberg

This program is free software; you can redistribute it and/or
modify it under the terms of Version 2 of the GNU General Public
License as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

Version 1.5, cvs $Revision: 1.14 $
SUMP Pump version 1.2.3, svn: 130