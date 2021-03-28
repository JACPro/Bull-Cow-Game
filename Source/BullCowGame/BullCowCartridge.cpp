// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

/*
Called when the game starts
*/
void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

/*
Called when the player presses Enter
*/
void UBullCowCartridge::OnInput(const FString& PlayerInput)
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else if (PlayerInput == HiddenWord)
    {
        PrintLine(TEXT("Correct! You won!"));        
        EndGame();
    }
    else
    {
        ProcessGuess(PlayerInput);                        
    }
}

/*
Welcome the player and start the game
*/
void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()) - 1];
    bGameOver = false;
    Lives = HiddenWord.Len();

    PrintLine(TEXT("--- Welcome to the Bull Cow Game! ---"));
    PrintLine(TEXT("You have to guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    /*-- DEBUG LINE-- */ PrintLine(FString::Printf(TEXT("The hidden word is: %s"), *HiddenWord)); /*-- DEBUG LINE-- */
    PrintLine(TEXT("Type something and press enter:"));

    const TCHAR HW[] = TEXT("orange");
}

/*
Stop player guessing and give the option to restart
*/
void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again."));
}

/*
Check if the player's guess is valid and correct
*/
void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    /*
    Check if the guess is the right length
    */
    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("The hidden word is %i characters long. Try again."), HiddenWord.Len());
        return;
    }

    /*
    Check if the guess has any repeating letters (i.e is an isogram)
    */
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("The word has no repeating characters. Try again."));
        return;
    }

    /*
    If the player has no lives left
    */
    if (Lives <= 0) {
        ClearScreen();
        PrintLine(TEXT("You ran out of lives! You lost!"));
        PrintLine(TEXT("The hidden word was \"%s\"."), *HiddenWord);
        EndGame();
    }

    /*
    If guess is valid but incorrect
    */
    --Lives;
    FBullCowCount Count = GetBullCows(Guess);
    
    PrintLine(TEXT("Bulls: %i\nCows: %i."), Count.Bulls, Count.Cows);
    PrintLine(TEXT("Incorrect! You have lost a life."));
    PrintLine(TEXT("You have %i lives left."), Lives);
    return;
}

/*
Check if the word has any repeating letters
*/
bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len() - 1; i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

/*
Take in a list of words and return the same word list minus those that don't adhere to our set rules
RULES:
1. Must be at least 4 letters long
2. Must be 8 letters or less
3. Must be an Isogram
*/
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        /*
        Check if letter in guess matches same index of Hidden word
        If so, the letter is a Bull
        */
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        /*
        If not a Bull, check if letter in Guess at current index matches
        ANY letter in Hidden Word        
        If so, the letter is a Cow
        */
        for (int CowCheckIndex = 0; CowCheckIndex < Guess.Len(); CowCheckIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[CowCheckIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}