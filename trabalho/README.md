# prog_arquivo
Trabalho programação com arquivos 

Um banco está fazendo uma auditoria nas transações internas realizadas nos últimos anos. A base de dados é composta por um arquivo CSV, contendo a cada linha: o dia, o mês e ano da transação, a agência e a conta que movimentou o dinheiro, o valor movimentado (valores positivos significam depósitos, valores negativos significam saques), a agência e conta para onde o dinheiro foi enviado (ou de onde veio). Essas duas últimas informações são opcionais; caso não constem na linha do arquivo CSV, a transação envolveu dinheiro vivo (depósito ou saque em espécie).
O auditor fiscal responsável quer realizar consultas na base de dados, mas também quer evitar repeti-las caso já tenham sido feitas anteriormente. A consulta consiste em indicar um mês e um ano e recuperar da base de dados a movimentação consolidada daquele mês. A movimentação consolidada possui as seguintes informações: o número da agência e da conta que teve alguma transação naquele mês/ano, o subtotal de dinheiro movimentado em espécie, o subtotal de dinheiro movimentado em transações eletrônicas e número o total de transações realizadas durante o mês/ano. Cada vez que uma consulta de movimentação consolidada é realizada pela primeira vez, os resultados são mostrados na tela, são também salvos em um arquivo binário “consolidadasMMAAAA.bin” (MM indica o mês a AAAA indicado o ano) e um arquivo de LOG é atualizado para indicar que a movimentação consolidada foi calculada para aquele mês/ano. Se uma nova consulta é solicitada para um mês/ano que já tenham sido analisados, o programa deve buscar os resultados apenas no arquivo binário ao invés de buscar na base completa e mostrá-los na tela.
O auditor também quer filtrar as movimentações consolidadas, visualizando apenas as que tiverem movimentado pelo menos X reais em espécie E/OU tiverem movimentado Y reais em transações eletrônicas em um determinado mês/ano. (OBS: a filtragem pode ser do tipo E ou do tipo OU). Sempre que uma filtragem for solicitada, o programa deve checar se a movimentação consolidada daquele mês/ano já foi feita (e fazê-la em caso negativo). Após ter certeza de que foi feita (basta verificar a existência do arquivo binário correspondente), a filtragem pode ser realizada solicitando ao usuário o valor de X, Y e o tipo de filtragem (E/OU).
O resultado da filtragem deve ser mostrado na tela e o programa deve inserir uma linha no arquivo de LOG informando a data e o horário da filtragem, os parâmetros X, Y e (E/OU), além da quantidade de registros mostrados na tela.


dia, mes, ano, agencia, conta, transacao, agencia_destino, conta_destino

DIA
MÊS
ANO
AGÊNCIA
CONTA
VALOR MOVIMENTADO (positivo = depósito (conta aumentou o valor), negativo = saque (diminui valor da conta))
AGÊNCIA DESTINO
CONTA DESTINO