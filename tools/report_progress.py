#!/usr/bin/env python3

import subprocess
import json
import re
import os

import mapfile_parser

def save(data, filename):
    directory = os.path.dirname(filename)
    if not os.path.exists(directory):
        os.makedirs(directory)
    with open(filename, "w") as outfile:
        json.dump(data, outfile)

def getColor(percentage):
    if percentage >= 75:
        return "lime"
    elif percentage >= 50:
        return "yellow"
    if percentage >= 25:
        return "darkorange"
    else:
        return "crimson"

def parse_progress(output):
    lines = output.strip().split("\n")
    all_of_folder_percent = 0
    sdk_of_total_percent = 0
    folder_percentages = {}

    for line in lines[1:]:  # Skip the header
        parts = re.split(r'\s{2,}', line.strip())  # Split by 2 or more spaces
        if len(parts) >= 5:
            category = parts[0]
            try:
                of_folder_percent = float(parts[4].strip('%'))
                folder_percentages[category] = of_folder_percent

                if category == "all":
                    all_of_folder_percent = of_folder_percent
                elif category == "sdk":
                    sdk_of_total_percent = float(parts[7].strip('()%'))
            except ValueError:
                print(f"Error on line: {line}")

    total_percentage = all_of_folder_percent + sdk_of_total_percent
    return total_percentage, folder_percentages

def main():
    # Execute the command and capture the output
    command = ["python3", "-m", "mapfile_parser", "progress", "build/SCPS_150.17.map", "asm", "asm/nonmatchings/"]
    result = subprocess.run(command, stdout=subprocess.PIPE, text=True)

    #result = mapfile_parser.frontends.progress.doProgress("build/SCPS_150.17.map", "asm", "asm/nonmatchings/")

    # Parse the output
    total_percentage, folder_percentages = parse_progress(result.stdout)

    # Print the results
    # print(f"Total percentage: {total_percentage:.4f}%")
    total_report = {
        "schemaVersion": 1,
        "label": f"Total percentage",
        "message": f"{total_percentage:.4f}%",
        "color": getColor(float(total_percentage)),
    }
    save(total_report, "progress/total_progress.json")
    
    for category, percent in folder_percentages.items():
        if category not in ["sdk", "all"]: # Skip the sdk and all categories
            print(f"{category} percentage: {percent:.4f}%")
            report = {
                "schemaVersion": 1,
                "label": f"{category}",
                "message": f"{percent:.4f}%",
                "color": getColor(float(percent)),
            }

            save(report, f"progress/{category}_progress.json")

if __name__ == "__main__":
    main()
