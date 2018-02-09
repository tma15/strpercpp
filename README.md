# strperc++
A C++ implementation of structured perceptron

## Requirements
- [googletest](https://github.com/google/googletest/tree/master/googletest) (Optional. This is used for unit tests.)
- [gperftools](https://github.com/gperftools/gperftools) (Optional. This is used for profiling)

## Install
```sh
$ autoreconf -iv
$ ./configure --prefix=/path/to/install
$ make
$ make install
```


## Data format
```
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
<FEATURE_1>[SPACE]<FEATURE_2>[SPACE] …  [SPACE]<LABEL>
…

```

## Template format

## Examples
### CONLL2000
You can download one of famous benchmark data of text chunking as follows:
```sh
$ cd scripts
$ ./download_conll2000.sh
```

Then, a model is learned and an output is obtained as follows:
```sh
$ train_strpercpp -e 3 ./data/train.txt template.conll model
$ test_strpercpp -v model ./data/test.txt > output.txt
```

Evaluation metrics are calculated as follows:

```
$ ./scripts/conlleval -d "        " < output.txt
processed 47377 tokens with 23852 phrases; found: 24221 phrases; correct: 22157.
accuracy:  95.38%; precision:  91.48%; recall:  92.89%; FB1:  92.18
             ADJP: precision:  69.35%; recall:  70.78%; FB1:  70.06  447
             ADVP: precision:  79.48%; recall:  80.48%; FB1:  79.98  877
            CONJP: precision:  36.36%; recall:  44.44%; FB1:  40.00  11
             INTJ: precision: 100.00%; recall:  50.00%; FB1:  66.67  1
              LST: precision:   0.00%; recall:   0.00%; FB1:   0.00  0
               NP: precision:  91.22%; recall:  92.92%; FB1:  92.06  12654
               PP: precision:  96.74%; recall:  97.92%; FB1:  97.32  4870
              PRT: precision:  77.68%; recall:  82.08%; FB1:  79.82  112
             SBAR: precision:  88.01%; recall:  85.05%; FB1:  86.50  517
               VP: precision:  91.91%; recall:  93.37%; FB1:  92.63  4732
```

Results of existing models are listed in [this page](https://www.clips.uantwerpen.be/conll2000/chunking/).

## Development
For unit testing,
```sh
$ autoreconf -iv
$ ./configure --prefix=/path/to/install --with-gtest=/path/to/gtest
$ make check
```

Profiling result is obtained as follows:
```sh
$ autoreconf -iv
$ ./configure --prefix=/path/to/install --with-gperf=/path/to/gperf
$ make
$ make install
$ export CPUPROFILE=prof.out; time /path/to/install/bin/train_strpercpp [options]
$ pprof /path/to/install/bin/train_strpercpp prof.out
```

## References
- Michael Collins, "Discriminative Training Methods
for Hidden Markov Models: Theory and Experiments
with Perceptron Algorithms.", EMNLP, 2002.
- Michael Collins and Brian Roark, "Incremental Parsing with the Perceptron Algorithm", ACL, 2004.
