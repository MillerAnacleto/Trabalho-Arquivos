#############################################
#                                           #
#             Código feito por              #
#               -- Miller --                #
#                                           #
#       Por favor, não tente entender       #
#                                           #
#############################################

import os
import argparse

ap = argparse.ArgumentParser(description='Faz os teste para o trabalho de Arquivos')
ap.add_argument("testes", default='all', help='Quais casos serão testados [all][(int)]')
ap.add_argument('-s', '--silent', action='store_true', help='Não mostra as diferenças caso arquivos de saída sejam diferentes')
ap.add_argument('-hd', '--hexdump', action='store_true', help='Abre o diff do vscode se os arquivos de saída forem diferentes')
args = vars(ap.parse_args())

if(args['testes'] == 'all'):
    primeiro_teste = 1
    ultimo_teste = 9
else:
    primeiro_teste = int(args['testes'])
    ultimo_teste = int(args['testes'])



copiar_arquivos = ''

for i in range(primeiro_teste, ultimo_teste + 1):
    copiar_arquivos += f'cp arquivos/antes/binario{i}.bin bin/;'
    if i > 3:
        copiar_arquivos += f'cp arquivos/antes/indice{i}.bin bin/;'

os.system(copiar_arquivos)

comando_make = 'make all'
os.system(comando_make)

for i in range(primeiro_teste, ultimo_teste + 1):
    executar = f'make run < in/{i}.in > teste.out'

    os.system(executar)

    conteudo = os.popen(f'diff out/{i}.out teste.out').read()

    if(conteudo == ''):
        resultado = 'Correto'
        print(f'caso {i}: {resultado}')
    else:
        resultado = 'Incorreto'
        print(f'caso {i}: {resultado}\n')
        if(not args['silent']):
            os.system(f'diff out/{i}.out teste.out -y')
        if(args['hexdump']):
            os.system(f'hexdump -Cv arquivos/depois/binario{i}.bin > bin{i}_experado.hd')
            os.system(f'hexdump -Cv bin/binario{i}.bin > bin{i}_obtido.hd')
            os.system(f'hexdump -Cv arquivos/depois/indice{i}.bin > indice{i}_experado.hd')
            os.system(f'hexdump -Cv bin/indice{i}.bin > indice{i}_obtido.hd')
            os.system(f'code --diff bin{i}_obtido.hd bin{i}_experado.hd')
            os.system(f'code --diff indice{i}_obtido.hd indice{i}_experado.hd')
            input('Precione ENTER para continuar os testes')
            os.system('rm *.hd')
    
    print('--------------//-------------')


os.system('rm teste.out')