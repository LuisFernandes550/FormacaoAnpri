#  Licenciamento MIT
#  Autor: Fernando Rui Campos - 25/03/2024
# foi necessário instalar a livraria DASH para simular um servidor de froma a não existirem janelas multiplas abertas
# utilizada a livraria Pandas para importação do ficheiro excel dos dados e organização
# utilizada a livraria  plotly para visualização e Dash para servidor local e visualização correta
# Deve abrir browser no endereço 127.0.0.1:8050
# No caso do Arduino não se pode ativar a porta monitor uma vez que o Python deixa de ter acesso à COMX
# realizado teste com  um máximo de 1000 registos sem problemas
# por cada registo é aproximadamente 1 segundo, ou em milisegundos na representação do eixo dos xx (TimeStamp)
# Se existir histórico de ficheiros basta colocar com o formato correto e substituir o ficheiro, de seguida fazer
# refresh no Browser, que irá ler o ficheiro "dadosSerieTL.xlsx"

import plotly.graph_objs as go
from plotly.subplots import make_subplots
import pandas as pd
import time
import dash
from dash import dcc, html  # Atualizado aqui
from dash.dependencies import Input, Output
import serial

# Abrir a porta serial COM3 com uma taxa de transmissão de 9600 bps
ser = serial.Serial('COM3', 9600)

# Verificar se a porta serial está aberta
if ser.is_open:
    print("Porta serial COM3 aberta com sucesso!")
else:
    print("Não foi possível abrir a porta serial COM3.")

# Você pode realizar operações de leitura e escrita na porta serial aqui

# Fechar a porta serial quando terminar de usá-la

file_path = 'dadosSerieTL.xlsx'
# Função para ler os dados da porta serial e escrevê-los no arquivo Excel

num_registros = 100



def write_to_excel(file_path):
    # Criar listas vazias para armazenar os dados
    timestamps = []
    tensoes = []
    temperaturas = []
    luminosidades = []

    # Contador de registros
    registros: int = 0

    # Ler os dados da porta serial enquanto estiverem disponíveis
    while registros < num_registros:

        if ser.is_open and ser.in_waiting > 0:
            # Ler uma linha da porta serial
            data = ser.readline().decode().strip()
            # Separar os valores da linha (assumindo que estão separados por vírgula)
            timestamp_str, tensao_str, temperatura_str, luminosidade_str = data.split(',')

            # Converter os valores para os tipos corretos
            timestamp = float(timestamp_str)
            tensao = float(tensao_str)
            temperatura = float(temperatura_str)
            luminosidade = float(luminosidade_str)

            # Adicionar os valores às listas
            timestamps.append(timestamp)
            tensoes.append(tensao)
            temperaturas.append(temperatura)
            luminosidades.append(luminosidade)

            # Incrementar o contador de registros

            # Incrementar o contador de registros
            registros += 1
            print(registros)
        else:
            # Se não há dados disponíveis, pausar por um curto período antes de verificar novamente
            time.sleep(0.3)  # Aguarda 0.1 segundo

            # Criar um DataFrame com os dados
        df = pd.DataFrame(
            {'Timestamp': timestamps, 'Tensao': tensoes, 'Temperatura': temperaturas, 'Luminosidade': luminosidades})

        # Escrever os dados no arquivo Excel
        df.to_excel(file_path, index=False)


# Chamar a função para escrever os dados no arquivo Excel
write_to_excel(file_path)

# Fechar a conexão da porta serial
ser.close()

# Inicializar a aplicação Dash
app = dash.Dash(__name__)

# Definir o layout da aplicação e definir a respetiva atualização do gráfico~e
app.layout = html.Div([
    dcc.Graph(id='live-graph', animate=True),
    dcc.Interval(
        id='graph-update',
        interval=5 * 1000  # em milissegundos
    ),
])


# Função para atualizar o gráfico
@app.callback(
    Output('live-graph', 'figure'),
    [Input('graph-update', 'n_intervals')]
)
def update_graph_scatter(n):
    try:

        df = pd.read_excel(file_path)  # Ler os dados do arquivo Excel e saltar de cada vez que l

        # Criar um novo objeto Figure com subplots
        fig = make_subplots(rows=2, cols=1, shared_xaxes=True, subplot_titles=("Temperatura  ºC", "Luminosidade"))

        # Adicionar gráfico de temperatura
        fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Temperatura'], mode='lines', name='Temperatura'), row=1, col=1)

        # Adicionar gráfico de luminosidade
        fig.add_trace(go.Scatter(x=df['Timestamp'], y=df['Luminosidade'], mode='lines', name='Luminosidade'), row=2, col=1)

        # Atualizar layout com títulos e rótulos dos eixos
        fig.update_layout(title='Dados de Temperatura e Luminosidade', xaxis=dict(title='Tempo'), yaxis=dict(title='Valor'))

        return fig
    except FileNotFoundError:
        # Se o arquivo Excel não for encontrado, imprima uma mensagem de aviso e retorne None
        print("O arquivo Excel não foi encontrado. Verifique se o arquivo existe e tente novamente.")
        return None


# Rodar a aplicação
if __name__ == '__main__':
    app.run_server(debug=False)

# Rodar a aplicação
if __name__ == '__main__':
    app.run_server(debug=False)
# Loop para atualizar o gráfico em uma única janela
while True:
    # Criar o gráfico de linhas com os dados atualizados
    n = 1
    update_graph_scatter(n)
    # Aguardar um intervalo de tempo antes de atualizar novamente
    time.sleep(10)

    # Intervalo de atualização em segundos (ajuste conforme necessário)
