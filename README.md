# E-Portfolio

## What did I change in Software test?

I added a pom.xml and organized the architecture so that it supports regression testing. This is a very good way to run tests continuously to make sure new changes don't change old functionality. I personally use `mvn clean package`, we can also inject dependencies so that the program runs and we can test new functionalities. By using the command `mvn clean package dependency:copy-dependencies`.
