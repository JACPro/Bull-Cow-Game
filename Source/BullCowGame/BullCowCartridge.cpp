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

    /*const FString WordListPath = FPaths::ProjectContentDir() / TEXT ("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);*/

    SetupGame();

    PrintLine(TEXT("The number of possible words is %i"), Words.Num());
    PrintLine(TEXT("The hidden word is: %s."), *HiddenWord); //DEBUG

    TArray<FString> ValidWords = GetValidWords(Words);

    for (int32 i = 0; i < ValidWords.Num(); i++)
    {
        PrintLine(TEXT("%s"), *ValidWords[i]);
    }
}

/*
Called when the player presses Enter
*/
void UBullCowCartridge::OnInput(const FString& Input)
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else if(Input == HiddenWord)
    {
        PrintLine(TEXT("Correct! You won!"));        
        EndGame();
    }
    else
    {
        ProcessGuess(Input);                        
    }
}

/*
Welcome the player and start the game
*/
void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("orange");
    bGameOver = false;
    Lives = HiddenWord.Len();

    /*-- DEBUG LINE-- */ PrintLine(FString::Printf(TEXT("The hidden word is: %s"), *HiddenWord)); /*-- DEBUG LINE-- */

    PrintLine(TEXT("--- Welcome to the Bull Cow Game! ---"));
    PrintLine(TEXT("You have to guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
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
void UBullCowCartridge::ProcessGuess(FString Guess)
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
    If guess is valid but incorrect
    */
    PrintLine(TEXT("Incorrect! You have lost a life."));
    --Lives;
    if (Lives > 0)
    {
        PrintLine(TEXT("You have %i lives left."), Lives);
        return;
    }
    
    /*
    If the player has no lives left
    */
    ClearScreen();
    PrintLine(TEXT("You ran out of lives! You lost!"));
    PrintLine(TEXT("The hidden word was \"%s\"."), *HiddenWord);
    EndGame();
}

/*
Check if the word has any repeating letters
*/
bool UBullCowCartridge::IsIsogram(FString Word) const
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

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> Words) const
{
    TArray<FString> ValidWords;

    for (int32 i = 0; i < Words.Num(); i++)
    {
        if (Words[i].Len() >= 4 && Words[i].Len() <= 8)
        {
            if (IsIsogram(Words[i]))
            {
                ValidWords.Emplace(Words[i]);
            }
        }
    }

    return ValidWords;
}