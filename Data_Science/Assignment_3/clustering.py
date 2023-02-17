import numpy as np 
import pandas as pd
import sys
import math

sys.setrecursionlimit(10**6)

epsilon = 0.0
num_of_cluster = 0
minpts = 0
idx_of_cluster = 0

def load_dataset():
    global data, num_of_cluster, epsilon, minpts

    data = pd.read_csv(argv[1], sep='\t', names=['object_id', 'x_coordinate', 'y_coordinate'])
    data.set_index('object_id', inplace=True)
    data['visited'] = 'False'
    data['noise'] = 'False'
    data['idx_of_cluster'] = 0

    num_of_cluster = argv[2]
    epsilon = argv[3]
    minpts = argv[4]


def build_dbscan():
    global data, epsilon, minpts, idx_of_cluster

    for index, row in data.iterrows():
        if (data.loc[index, 'visited'] == 'False'):
            data.loc[index, 'visited'] = 'True'
 
            neighbors = find_neighbors(index, row)

            if(is_corepoint(neighbors)):
                idx_of_cluster += 1
                data.loc[index, 'idx_of_cluster'] = idx_of_cluster
                clustering_neighbors(neighbors)

            else:
                data.loc[index, 'noise'] = 'True'

    for index, row in data.iterrows():
        if(data.loc[index, 'noise'] == 'True'):
            data = data.drop(index)            
            


def find_neighbors(target_index, target_row):
    global epsilon
    
    target_x_coordinate = target_row['x_coordinate']
    target_y_coordinate = target_row['y_coordinate']

    false_data = data.loc[data['visited']=='False']
    np_false_index = false_data.index.to_numpy()
    np_false_row = false_data.to_numpy()

    neighbors = pd.DataFrame()

    for idx in range (len(np_false_row)):
        distance = math.sqrt(math.pow(target_x_coordinate-np_false_row[idx][0],2) + math.pow(target_y_coordinate - np_false_row[idx][1], 2))
        if (distance <= float(epsilon)):
            neighbors = neighbors.append(data.iloc[np_false_index[idx],:])
            data.loc[np_false_index[idx], 'visited'] = 'True'
            
    return neighbors
   
    


def is_corepoint(x):
    return len(x)>=float(minpts)


def clustering_neighbors(neighbors):

    np_neighbors_index = neighbors.index.to_numpy()
    np_neighbors_row = neighbors.to_numpy()

    for idx in range(len(np_neighbors_row)):
        data.loc[np_neighbors_index[idx], 'visited'] = 'True'
        data.loc[np_neighbors_index[idx], 'idx_of_cluster'] = idx_of_cluster

        neighbors2 = find_neighbors(np_neighbors_index[idx], data.iloc[np_neighbors_index[idx],:])

        if (is_corepoint(neighbors2)):
            clustering_neighbors(neighbors2)
        
    #for index, row in neighbors.iterrows():
        #data.loc[index, 'visited'] = 'True'
        #data.loc[index, 'idx_of_cluster'] = idx_of_cluster

        #neighbors2 = find_neighbors(index, row)

        #if (is_corepoint(neighbors2)):
            #clustering_neighbors(neighbors2)
   

def write_dataset():
    i = 0
   
    for idx_of_cluster in data['idx_of_cluster'].value_counts().index:

        file_name = str(argv[1])[0:-4] + '_cluster_' + str(i) + '.txt'

        if (i < int(num_of_cluster)):
            with open(file_name, 'w', encoding='UTF-8') as f:
                    
                for index, row in data.iterrows():
                    if (data.loc[index, 'idx_of_cluster'] == idx_of_cluster):
                        f.write(f'{index}\n')

        else:
            return
            
        i += 1


if __name__ == '__main__':

    argv = sys.argv
    load_dataset()

    build_dbscan()
    write_dataset()
