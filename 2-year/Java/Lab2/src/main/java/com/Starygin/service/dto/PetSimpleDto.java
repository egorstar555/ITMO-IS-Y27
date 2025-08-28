package com.Starygin.service.dto;

import com.Starygin.data.entity.PET_COLOR;

import java.time.LocalDate;

public record PetSimpleDto(Long id, String name, LocalDate birthday, PET_COLOR color) { }