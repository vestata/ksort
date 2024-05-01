import numpy as np

def calculate_z_scores(data):
    mean = sum(data) / len(data)
    std_dev = (sum((x - mean) ** 2 for x in data) / len(data)) ** 0.5
    z_scores = [(x - mean) / std_dev for x in data]
    return z_scores

def outlier_filter(data, threshold=1):
    z_scores = calculate_z_scores(data)

    normal_data = [data[i] for i, z in enumerate(z_scores) if abs(z) <= threshold]
    normal_mean = sum(normal_data) / len(normal_data) if normal_data else sum(data) / len(data)

    replaced_data = [data[i] if abs(z_scores[i]) <= threshold else normal_mean for i in range(len(data))]

    return replaced_data

if __name__ == '__main__':
    with open('out_ksort.csv', 'r') as file:
        lines = file.readlines()
    
    datas = []
    for line in lines:
        tmp = line.strip().split(",")  # 去除換行符並分割字符串
        datas.append(tmp)
    
    second_column = [int(row[1]) for row in datas]
    
    # 過濾 outliers
    filtered_second_column = outlier_filter(second_column)
    
    # 替換原始數據的第二列
    for i, value in enumerate(filtered_second_column):
        datas[i][1] = value

    # 儲存結果到新的 CSV 檔案
    with open('out_ksort_1.csv', 'w') as file:
        for row in datas:
            file.write(', '.join(map(str, row)) + '\n')