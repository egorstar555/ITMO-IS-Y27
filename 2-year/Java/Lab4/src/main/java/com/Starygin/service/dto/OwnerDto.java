package com.Starygin.service.dto;

import java.time.LocalDate;
import java.util.List;

public record OwnerDto(Long id, String name, LocalDate birthday, List<PetSimpleDto> pets) { }