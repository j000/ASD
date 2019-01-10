test: test15a
test: test15b
test: test100
test: test10k

test%: test%.txt output%.txt
	diff -sq $^

test%.txt: testTree.x input%.txt
	./testTree.x < input$*.txt > test$*.txt

mostlyclean: testclean
.PHONY: testclean
testclean:
	$(RM) test*.txt
