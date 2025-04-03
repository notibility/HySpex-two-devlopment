import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
    # 读取txt文件,从第二行开始读，读取的每行数据转成float
    path = r"E:\DataSet\usgs_splib07\ASCIIdata\ASCIIdata_splib07a\ChapterM_Minerals\splib07a_Actinolite_HS22.1B_ASDFRb_AREF.txt"
    with open(path, 'r', encoding='UTF-8') as f:
        lines = f.readlines()  # 接收数据
        # print(len(lines))
        head = lines[0]
        head_list = head.split()
        print(head_list)
        float_spectal = []
        value_to_remove = -1.23e+034
        new_list = list(filter(lambda x: float(x) != value_to_remove, lines[1:]))
        print(new_list)
        for line in new_list:  # 遍历数据
            float_spectal.append(float(line))
        min_value = np.min(float_spectal)
        max_value = np.max(float_spectal)
        print(min_value, max_value)
        normal_spectal = (np.array(float_spectal) - min_value) / (max_value - min_value)  # 转成numpy数组
        print(normal_spectal)
        plt.plot(normal_spectal)
        plt.title(head_list[2])
        plt.xlabel('Bands')
        plt.ylabel('Relative spectral response')
        plt.savefig(path.replace('.txt', '.png'))
        plt.show()