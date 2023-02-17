import numpy as np 
import pandas as pd
import sys
import math
from pprint import pprint
from collections import defaultdict

order_split_feature = np.array([])

def load_training_data():
    global training_data, features, target_feature, original_data
    
    training_data = pd.read_csv(argv[1], sep='\t', header=0)
    original_data = training_data
    features = np.array(training_data.columns[0:-1])
    target_feature = np.array(training_data.columns[-1])


def calculate_entropy(target_columns):
    elements, counts = np.unique(target_columns, return_counts = True)
    entropy = -np.sum([(counts[i]/np.sum(counts))*np.log2(counts[i]/np.sum(counts)) for i in range(len(elements))])
    return entropy

    
def select_split_feature(feature):
    global training_data, target_feature, order_split_feature

    information_gain = {}
    
    entropy = calculate_entropy(training_data[target_feature])

    for j in feature:
        vals,counts= np.unique(training_data[j],return_counts=True)
        weighted_entropy = np.sum([(counts[i]/np.sum(counts))*
                                   calculate_entropy(training_data.where(training_data[j]==vals[0]).dropna()[target_feature])
                                   for i in range(len(vals))])

        information_gain[j] = entropy - weighted_entropy

    if max(information_gain, key=information_gain.get) not in order_split_feature:
        order_split_feature= np.append(order_split_feature, max(information_gain, key=information_gain.get))
        
    return max(information_gain, key=information_gain.get)
    


def build_tree(data, feature_name, parent_label):
    global target_feature
    
    if len(np.unique(data[target_feature])) < 2:
        return np.unique(data[target_feature])[0]

    elif len(feature_name) == 0:
        return parent_label

    else:
        
        selected_feature = select_split_feature(feature_name)

        dt = {selected_feature:{}}
        feature_name = [i for i in feature_name if i != selected_feature]
        
        parent_label = np.unique(data[target_feature])\
                       [np.argmax(np.unique(data[target_feature], return_counts=True)[1])]

        for i in np.unique(data[selected_feature]):
            divided_data = data.where(data[selected_feature] == i).dropna()

            child_tree = build_tree(divided_data, feature_name, parent_label)
            dt[selected_feature][i] = child_tree
            
        return(dt)


def get_label_average(feature, inputs):

    idx = np.where(order_split_feature == feature)
  
    data = training_data
    
    for i in order_split_feature[0:int(idx[0][0])]:
        data = data.where(data[i] == inputs[i]).dropna()

    target_count = data[target_feature].value_counts()
    
    label = np.argmax(data[target_feature])

    return target_count.index[target_count == max(target_count)].tolist()[0]


    
def load_test_data():
    global test_data, result_data
    
    test_data = pd.read_csv(argv[2], sep='\t', header=0)
    result_data = test_data.to_dict('records')



def classify_label(dt, inputs):

    feature = ''.join(list(dt.keys()))
    subtree_dict = list(dt.values())[0]
    inputs_value = subtree_dict.get(inputs.get(feature))

    if inputs_value == None:
        return get_label_average(feature, inputs)
        
    if type(inputs_value) is not dict:
        return inputs_value

    return classify_label(inputs_value, inputs)

    

def write_txtfile():
    with open(argv[3], 'w', encoding='UTF-8') as f:
        for feature in features:
            f.write(f'{feature}\t')
        f.write(f'{target_feature}\n')
        
        for i in result_data:
            label = classify_label(tree, i)
            
            for value in i.values():
                f.write(f'{value}\t')
            f.write(f'{label}')
            f.write(f'\n')
    

if __name__ == '__main__':

    argv = sys.argv
    training_data = argv[1]
    test_data = argv[2]

    load_training_data()

    tree = build_tree(training_data, features, '')

    load_test_data()
    
    
    write_txtfile()
        

