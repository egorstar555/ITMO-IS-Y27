package com.Starygin.Application.Models;

public record User(long id, String userName, String password, UserRole role) { }