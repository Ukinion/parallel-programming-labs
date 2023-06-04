#pragma once

#include <vector>
#include <iostream>

#include "../constants.h"
#include "../exception/invalid_option_exception.h"
#include "../exception/out_of_exception.h"
#include "../exception/not_comparable_exception.h"

class GameField {
public:
    GameField() = default;
    GameField(const GameField&) = default;
    GameField& operator=(const GameField&) = default;
    void InitGameField(int, int);
    void CreateGlider(int, int);
    bool IsOutOfField(int x, int y) const;
    void NextGeneration(const GameField&);
    friend bool operator==(const GameField&, const GameField&);
    void PrintField() const;
    std::vector<char> GetField() const;
    void SetNewField(const std::vector<char>&, int ,int, int, int);
private:
    int CountAliveNeighbours(int, int, const std::vector<char>&) const;
    int CountAliveNeighbours(int x, int y, const std::vector<char>&, int, const std::vector<char>&) const;
    bool IsInvalidFieldOptions(int, int) const;
    static bool IsNotComparable(const GameField&, const GameField&);
    int mod(int, int) const;
    void SendRowPrevProcess() const;
    void SendRowNextProcess() const;
    void ReceiveNextProcessRow();
    void ReceivePrevProcessRow();
    void UpdateFieldMiddle(const std::vector<char>&);
    void UpdateFieldTop(const std::vector<char>&);
    void UpdateFieldBottom(const std::vector<char>&);
    void InitMetaMPI(int, int);
    struct DataMPI {
        int rank;
        static int size;
        static std::vector<char> first_row;
        static std::vector<char> last_row;
        MPI_Request* send_first_row;
        MPI_Request* send_last_row;
        MPI_Request* recv_first_row;
        MPI_Request* recv_last_row;
    };
    DataMPI mpi_scheduler_;
    std::vector<char> game_field_;
    int row_;
    int col_;
};