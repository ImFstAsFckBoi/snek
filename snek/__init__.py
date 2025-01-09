from site import getsitepackages
from os import path
from sysconfig import get_path
from typing import List


def pkg_config() -> List[str]:
    out = []
    out.append(get_path("include"))
    for sites in getsitepackages(): 
        out.append(path.join(sites, "snek", "include"))
    return out