from . import pkg_config

if __name__ == "__main__":
    print(
        " ".join(
            (f"-I{i}" for i in pkg_config())
        )
    )
