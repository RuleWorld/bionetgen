import yaml

with open('hyperparameters.yml') as f:
    # use safe_load instead load
    dataMap = yaml.safe_load(f)

print dataMap
