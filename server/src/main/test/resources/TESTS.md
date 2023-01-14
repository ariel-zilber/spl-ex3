# Tests for client

## TEST 1

### Steps
```
login 127.0.0.1:7777 meni films
```
```
join germany_japan
```
```
report /media/ariel/k8s/spl/ex3_v2/SPL231-Assignment3-student-template-2/client/data/events1.json
```

```
summary germany_japan meni events1_out.json
```
## TEST 2

test not login at first
### Steps
```
join germany_japan
exit germany_japan
summary germany_japan meni events1_out.json
report /media/ariel/k8s/spl/ex3_v2/SPL231-Assignment3-student-template-2/client/data/events1.json
```

## TEST 3
test login,subscribe then unsubscribe
### Steps
```
login 127.0.0.1:7777 meni films
```
```
join germany_japan
```
```
exit germany_japan
```
## TEST 4
test login,then attempt to report  not subscribe event
### Steps
```
login 127.0.0.1:7777 meni films
```

```
report /media/ariel/k8s/spl/ex3_v2/SPL231-Assignment3-student-template-2/client/data/events1.json
```
## TEST 5
test login,then attempt to exit not subscribe event
### Steps
```
login 127.0.0.1:7777 meni films
```
```
exit germany_japan
```

## TEST 6
test login,then attempt to exit not subscribe event
### Steps
```
login 127.0.0.1:7777 meni films
```
```
join germany_japan
```
```
exit germany_japan
```
```
join germany_japan
```


## TEST 6
test subscribe multiple times to same topic
### Steps
```
login 127.0.0.1:7777 meni films
```
```
join germany_japan
```
```
join germany_japan
```


## TEST 7
test subscribe multiple times to same topic
### Steps
```
login 127.0.0.1:7777 meni films
```
```
join germany_japan
```
```
join germany_japan
```

## TEST 8
enter random shit,not real commands before login

### Steps
```
lfds
```
```
dcdcgh
```
## TEST 9
enter random shit,not real commands after login

### Steps
```
login 127.0.0.1:7777 meni films
```
```
lfds
```
```
dcdcgh
```


## TEST 9
Enter wrong arguments count

### Steps
```
login 127.0.0.1:7777 meni films
```
```

```

```

```
## TEST 10
login,logout then again login should allow 


### Steps
```
login 127.0.0.1:7777 meni films
```
```
logout
```

```
login 127.0.0.1:7777 meni films
```
## TEST 10
just logout
### steps
```
logout

```

## TEST 11
login,subscribe,logout,exit
### steps
```
logout

```

## TEST 12
two user login same account
### steps
#### user 1
```
login 127.0.0.1:7777 meni films
```
#### user 2
```
login 127.0.0.1:7777 meni films
```

## TEST 13
two user login 
### steps
#### user 1
```
login 127.0.0.1:7777 mario films
join germany_japan
```
#### user 2
```
login 127.0.0.1:7777 luige films
join germany_japan
summary germany_japan mario events1_out2.json
```
```
report /media/ariel/k8s/spl/ex3_v2/SPL231-Assignment3-student-template-2/client/data/events1.json
```
```

summary germany_japan mario events1_out.json
```

#### user 3
```
login 127.0.0.1:7777 wario films
join germany_japan
summary germany_japan mario events1_out2.json
```
```
report /media/ariel/k8s/spl/ex3_v2/SPL231-Assignment3-student-template-2/client/data/events1.json
```
```

summary germany_japan mario events1_out.json
```

```
exit germany_japan

```


## TEST 14

### Steps
```
login 127.0.0.1:7778 meni films
```

## TEST 15

### Steps
```
login 127.0.0.1:7778 meni films
```
## TEST 16

### Steps
```
login 127.0.0.1:7777 meni films
```
```
logout
```

```
login 127.0.0.1:7777 meni lovesbibi
```

```
login 127.0.0.1:7777 meni films
```
summary germany_japan meni xxxxxxxxxxx.json