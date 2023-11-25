def apply(config, args):
    config["arch"] = "mipsee"
    config["baseimg"] = f"iso/SCPS_150.17"
    config["myimg"] = f"build/SCPS_150.17"
    config["mapfile"] = f"build/SCPS_150.17.map"
    config["source_directories"] = [
        "src",
        "asm",
        "include",
        "assets",
    ]
    config["make_command"] = ["ninja"]
    config["expected_dir"] = f"expected/"