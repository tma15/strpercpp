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
$ train_strpercpp -e 10 ./data/train.txt template.conll model
$ test_strpercpp -v 1 -d 0 model ./data/test.txt > output.txt
```

Evaluation metrics are calculated as follows:

```
$ ./scripts/conlleval -d "        " < output.txt
processed 47377 tokens with 23852 phrases; found: 23843 phrases; correct: 22295.
accuracy:  95.85%; precision:  93.51%; recall:  93.47%; FB1:  93.49
             ADJP: precision:  77.54%; recall:  74.89%; FB1:  76.19  423
             ADVP: precision:  83.70%; recall:  80.02%; FB1:  81.82  828
            CONJP: precision:  55.56%; recall:  55.56%; FB1:  55.56  9
             INTJ: precision: 100.00%; recall:  50.00%; FB1:  66.67  1
              LST: precision:   0.00%; recall:   0.00%; FB1:   0.00  0
               NP: precision:  93.75%; recall:  93.70%; FB1:  93.72  12415
               PP: precision:  96.87%; recall:  97.92%; FB1:  97.40  4863
              PRT: precision:  77.98%; recall:  80.19%; FB1:  79.07  109
             SBAR: precision:  88.27%; recall:  85.79%; FB1:  87.01  520
               VP: precision:  93.56%; recall:  93.90%; FB1:  93.73  4675
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
