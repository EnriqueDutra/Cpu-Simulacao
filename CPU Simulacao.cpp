/************************************************************************
 *
 * Copyright (C) 2023 Enrique de Abreu Dutra. 
 * Todos Direitos Reservados.
 *
 * Proibida a reprodução, modificação e distribuição total ou parcial 
 * deste produto sem devida autorização do autor.
 *
 ************************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

using WORD = int; // 32 bits
using u32 = unsigned int;

struct RAM
{
    static constexpr int MAXIMO_MEM = 256;
    WORD Data[MAXIMO_MEM];

    void Initialise()
    {
        for (u32 i = 0; i < MAXIMO_MEM; i++)
        {
            Data[i] = 0;
        }
    }
};

struct CPU
{
    WORD R1, R2, R3, R4; // Registradores
    RAM memory;

    std::map<std::string, WORD*> registers; // Mapeia nomes de registradores para seus endereços

    CPU() : memory(), registers()
    {
        Reset();
    }

    // Inicializacao
    void Reset()
    {
        R1 = R2 = R3 = R4 = 0; // Registradores
        memory.Initialise();

        // Inicialize o mapa de registradores
        registers["R1"] = &R1;
        registers["R2"] = &R2;
        registers["R3"] = &R3;
        registers["R4"] = &R4;
    }

    // INSTRUCAO SHOWMEM
    void SHOWMEM(const RAM& memory)
    {
        for (u32 i = 0; i < RAM::MAXIMO_MEM; i++)
        {
            std::cout << "MEM[" << i << "]: " << memory.Data[i] << std::endl;
        }
    }

    // INSTRUCAO SHOW
    void SHOW()
    {
        std::cout << "R1: " << R1 << std::endl;
        std::cout << "R2: " << R2 << std::endl;
        std::cout << "R3: " << R3 << std::endl;
        std::cout << "R4: " << R4 << std::endl;
    }

    // INSTRUCAO HELP
    void HELP()
    {
        std::cout << "" << std::endl;
        std::cout << "Instrucoes que podem ser usadas: " << std::endl;
        std::cout << "" << std::endl;
        std::cout << "MOV R1 R2 - Move valor de R2 para R1" << std::endl;
        std::cout << "SUB R1 R2 - Subtrai valor de R2 com R1" << std::endl;
        std::cout << "ADD R1 R2 - Adiciona valor de R2 para R1" << std::endl;
        std::cout << "MUL R1 R2 R3 - Multiplica R2 e R3 e armazena o resultado em R1" << std::endl;
        std::cout << "SHOW - Mostra estados dos Registradores" << std::endl;
        std::cout << "SHOWMEM - Mostra estado da RAM" << std::endl;
        std::cout << "LOAD R2 54 - Carrega valor da memoria em um registrador (54 endereco RAM)" << std::endl;
        std::cout << "STORE R2 54 - Armazena valor de um registrador na memoria (54 endereco RAM)" << std::endl;
        std::cout << "SHUTDOWN - Desliga a CPU" << std::endl;
        std::cout << "RESET - Reinicia a CPU" << std::endl;
        std::cout << "" << std::endl;
    }

    // INSTRUCAO ADD
    void ADD(WORD& dest, const WORD value)
    {
        dest += value;
    }

    // INSTRUCAO SUB
    void SUB(WORD& dest, const WORD value)
    {
        dest -= value;
    }

    //INSTRUCAO MOV
    void MOV(WORD& dest, const WORD value)
    {
        dest = value;
    }

    //INSTRUCAO MUL
    void MUL(WORD& dest, const WORD src1, const WORD src2)
    {
        dest = src1 * src2;
    }

    void RESET()
    {
        // Reseta todos os registradores para zero
        R1 = R2 = R3 = R4 = 0;

        // Limpa a memória, definindo todos os valores para zero
        memory.Initialise();
    }
    
    //INSTRUCAO LOAD
    void LOAD(const std::string& reg, u32 address)
    {
        auto regIter = registers.find(reg);
        if (regIter != registers.end())
        {
            if (address >= 0 && address < RAM::MAXIMO_MEM)
            {
                *regIter->second = memory.Data[address]; // Carrega o valor da memória para o registrador
            }
            else
            {
                std::cout << "Endereço de memória inválido!" << std::endl;
            }
        }
        else
        {
            std::cout << "Registrador inválido!" << std::endl;
        }
    }

    //INSTRUCAO STORE
    void STORE(const std::string& reg, u32 address)
    {
        auto regIter = registers.find(reg);
        if (regIter != registers.end())
        {
            if (address >= 0 && address < RAM::MAXIMO_MEM)
            {
                memory.Data[address] = *regIter->second; // Armazena o valor do registrador na memória
            }
            else
            {
                std::cout << "Endereço de memória inválido!" << std::endl;
            }
        }
        else
        {
            std::cout << "Registrador inválido!" << std::endl;
        }
    }

};

int main()
{
    CPU cpu; // variavel local cpu que reverencia o struct CPU

    std::cout << "" << std::endl;
    std::cout << "##############################" << std::endl;
    std::cout << "Memoria RAM Inicializada." << std::endl;
    std::cout << "Registradores Inicializados." << std::endl;
    std::cout << "CPU Inicializado!" << std::endl;
    std::cout << "##############################" << std::endl;
    std::cout << "" << std::endl;

    std::string input; // Variavel tipo string

    // LOOP DE COMANDO
    while (true)
    {
        std::cout << "Digite um comando (HELP para ajuda): ";
        std::getline(std::cin, input);

        //Instrucoes Especiais
        // 
        // INSTRUCAO SHUTDOWN
        if (input == "SHUTDOWN" || input == "shutdown")
        {
            std::cout << "CPU Desligado" << std::endl;
            break;
        }

        //INSTRUCAO RESET - Reseta CPU
        if (input == "RESET" || input == "reset")
        {
            cpu.RESET();
            std::cout << "" << std::endl;
            std::cout << "########################" << std::endl;
            std::cout << "CPU Reiniciada" << std::endl;
            std::cout << "Memoria RAM Zerada." << std::endl;
            std::cout << "Registradores Zerados." << std::endl;
            std::cout << "########################" << std::endl;
            std::cout << "" << std::endl;
            continue; // Continue para o próximo ciclo do loop
        }

        std::istringstream iss(input); // iss da classe 'istringstream' permite ler dados de uma string
        std::string command, reg;      // command é a variavel que armazenara primeira parte do comando (add sub mov) // reg fica com a segunda parte(R1 R2 R3)
        WORD value;                   // Armazena o terceiro valor, usado na operacao

        iss >> command;

        // MOV, SUB, ADD
        if (command == "MOV" || command == "mov" || command == "SUB" || command == "sub" || command == "ADD" || command == "add")
        {
            iss >> reg >> value;

            if (!iss.fail())
            {
                std::transform(reg.begin(), reg.end(), reg.begin(), ::toupper); // Transform Converte para maiúsculas

                auto regIter = cpu.registers.find(reg); // Usa find de cpu.registers para procurar o registrador convertido em maiusculas

                if (regIter != cpu.registers.end())
                {
                    // Verifica se o registrador é válido e realiza a instrucao
                    //ADD
                    if (command == "ADD" || command == "add")
                    {
                        cpu.ADD(*regIter->second, value);
                    }

                    //SUB
                    else if (command == "SUB" || command == "sub")
                    {
                        cpu.SUB(*regIter->second, value);
                    }

                    //MOV
                    else if (command == "MOV" || command == "mov")
                    {
                        cpu.MOV(*regIter->second, value);
                    }
                }
                else
                {
                    std::cout << "Registrador inválido!" << std::endl;
                }
            }
            else
            {
                std::cout << "Uso inválido do comando " << command << ". Sintaxe correta: " << command << " <registrador> <valor>" << std::endl;
            }
        }

        // INSTRUCAO MUL - 3 VALORES
        else if (command == "MUL" || command == "mul")
        {
            std::string destReg, src1Reg, src2Reg;
            iss >> destReg >> src1Reg >> src2Reg;

            if (!iss.fail())
            {
                std::transform(destReg.begin(), destReg.end(), destReg.begin(), ::toupper);
                std::transform(src1Reg.begin(), src1Reg.end(), src1Reg.begin(), ::toupper);
                std::transform(src2Reg.begin(), src2Reg.end(), src2Reg.begin(), ::toupper);

                auto destRegIter = cpu.registers.find(destReg);
                auto src1RegIter = cpu.registers.find(src1Reg);
                auto src2RegIter = cpu.registers.find(src2Reg);

                if (destRegIter != cpu.registers.end() && src1RegIter != cpu.registers.end() && src2RegIter != cpu.registers.end())
                {
                    cpu.MUL(*destRegIter->second, *src1RegIter->second, *src2RegIter->second);
                }
                else
                {
                    std::cout << "Registrador inválido!" << std::endl;
                }
            }
            else
            {
                std::cout << "Uso inválido do comando MUL. Sintaxe correta: MUL <dest_reg> <src1_reg> <src2_reg>" << std::endl;
            }
        }

        //SHOW
        else if (command == "SHOW" || command == "show")
        {
            cpu.SHOW();
        }

        //SHOWMEM
        else if (command == "SHOWMEM" || command == "showmem")
        {
            cpu.SHOWMEM(cpu.memory);
        }

        //INSTRUCAO LOAD
        else if (command == "LOAD" || command == "load")
        {
            std::string reg;
            u32 address;
            iss >> reg >> address;

            if (!iss.fail())
            {
                std::transform(reg.begin(), reg.end(), reg.begin(), ::toupper); // Transform Converte para maiúsculas

                auto regIter = cpu.registers.find(reg); // Usa find de cpu.registers para procurar o registrador convertido em maiusculas

                if (regIter != cpu.registers.end())
                {
                    cpu.LOAD(reg, address);
                }
                else
                {
                    std::cout << "Registrador inválido!" << std::endl;
                }
            }
            else
            {
                std::cout << "Uso inválido do comando LOAD. Sintaxe correta: LOAD <registrador> <endereço>" << std::endl;
            }
        }

        //INSTRUCAO STORE
        else if (command == "STORE" || command == "store")
        {
            std::string reg;
            u32 address;
            iss >> reg >> address;

            if (!iss.fail())
            {
                std::transform(reg.begin(), reg.end(), reg.begin(), ::toupper); // Transform Converte para maiúsculas

                auto regIter = cpu.registers.find(reg); // Usa find de cpu.registers para procurar o registrador convertido em maiusculas

                if (regIter != cpu.registers.end())
                {
                    cpu.STORE(reg, address);
                }
                else
                {
                    std::cout << "Registrador inválido!" << std::endl;
                }
            }
            else
            {
                std::cout << "Uso inválido do comando STORE. Sintaxe correta: STORE <registrador> <endereço>" << std::endl;
            }
        }

        //HELP
        else if (command == "HELP" || command == "help")
        {
            cpu.HELP();
        }
        else
        {
            std::cout << "Comando inválido!" << std::endl;
        }
    }

    return 0;
}
