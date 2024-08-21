def lz78_encode_bin(data):
    dictionary = {}
    current_string = ""
    code = []
    dict_size = 1

    for bit in data:
        temp_string = current_string + bit
        if temp_string in dictionary:
            current_string = temp_string
        else:
            if current_string == "":
                index = 0
            else:
                index = dictionary[current_string]
            
            code.append((index, bit))
            dictionary[temp_string] = dict_size
            dict_size += 1
            current_string = ""

    if current_string:
        code.append((dictionary[current_string], ""))

    # Converte a saída em uma sequência contínua de bits
    encoded_bits = ""
    index_size = 1  # Começa com 1 bit para o índice

    for i, (index, bit) in enumerate(code):
        if i > 0 and (2 ** index_size) <= i:
            index_size += 1  # Aumenta o tamanho dos índices conforme o dicionário cresce
        
        encoded_bits += f"{index:0{index_size}b}{bit}"

    return encoded_bits

# Exemplo de uso
# data = "00110110001111000"
data = "00101011101011010001111"
encoded_data = lz78_encode_bin(data)
print("Resultado codificado contínuo:", encoded_data)
