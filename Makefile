.PHONY: clean sdist wheel

wheel:
	python3 -m build --wheel

sdist:
	python3 -m build --sdist

clean:
	rm -rf testmod.so *.egg-info build dist testmod/*.egg-info testmod/build testmod/dist