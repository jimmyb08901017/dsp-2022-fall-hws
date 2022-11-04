#!/bin/bash
# Program:
#       This program automatically runs all train program and test and print accuracy.
# History:
# 2022/10/28	Jimmy
make

./train 100 ./model_init.txt data/train_seq_01.txt model_01.txt
./train 100 ./model_init.txt data/train_seq_02.txt model_02.txt
./train 100 ./model_init.txt data/train_seq_03.txt model_03.txt
./train 100 ./model_init.txt data/train_seq_04.txt model_04.txt
./train 100 ./model_init.txt data/train_seq_05.txt model_05.txt

./test modellist.txt data/test_seq.txt result.txt
./acc