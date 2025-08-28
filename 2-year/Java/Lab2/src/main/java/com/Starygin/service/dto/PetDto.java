package com.Starygin.service.dto;

import com.Starygin.data.entity.PET_COLOR;

import java.time.LocalDate;
import java.util.List;

public record PetDto(Long id, String name, LocalDate birthday, PET_COLOR color, OwnerSimpleDto owner,
                     List<PetSimpleDto> friends) { }